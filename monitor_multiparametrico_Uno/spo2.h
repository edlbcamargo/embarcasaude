/*******************************************************************************
 * Funções usadas com o sensor MAX30102 (oxímetro) para cálculo do SpO2.
 * As funções originais da Maxim foram modificadas para que o cálculo fosse
 * feito utilizando (muito) menos memória.
 * O cálculo da frequência cardíaca também foi removido das funções originais.
 * 
 * Funções para uso da biblioteca:
 * - void adicionaMedidaIR(int16_t medida): adiciona uma nova medida do led ir;
 * - void adicionaMedidaRed(int16_t medida): adiciona uma nova medida do led red;
 * - int16_t medeSpO2(): calcula e retorna valor do SpO2
 * 
 * Criado por: Erick León, 2023
 ********************************************************************************/

/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/

#define BUFFER_SIZE 32  // tamanho dos vetores de dados.
int16_t irBuffer[BUFFER_SIZE];   // vetor dos dados do LED ir
int16_t redBuffer[BUFFER_SIZE];  // vetor dos dados do LED vermelho
#define MA_SIZE 4 // tamanho da média móvel usada no filtro dos dados

/////////////////////////////////////////////////////////////////////
// Adiciona uma nova medida ao vetor de dados do LED IR.
// Esta função só roda quando chamada.
void adicionaMedidaIR(int16_t medida){
  // 'anda' com dados para esquerda e adiciona nova medida
  for (int idx=0; idx<BUFFER_SIZE-1;idx++) irBuffer[idx] = irBuffer[idx+1];
  irBuffer[BUFFER_SIZE-1] = medida;
}

/////////////////////////////////////////////////////////////////////
// Adiciona uma nova medida ao vetor de dados do LED vermelho.
// Esta função só roda quando chamada.
void adicionaMedidaRed(int16_t medida){
  // 'anda' com dados para esquerda e adiciona nova medida
  for (int idx=0; idx<BUFFER_SIZE-1;idx++) redBuffer[idx] = redBuffer[idx+1];
  redBuffer[BUFFER_SIZE-1] = medida;
}

/////////////////////////////////////////////////////////////////////
// Sort array in ascending order.
// Esta função só roda quando chamada.
void mmaxim_sort_ascend(int16_t  *pn_x, int16_t n_size){
  int16_t i, j, n_temp;
  for (i = 1; i < n_size; i++) {
    n_temp = pn_x[i];
    for (j = i; j > 0 && n_temp < pn_x[j-1]; j--)
        pn_x[j] = pn_x[j-1];
    pn_x[j] = n_temp;
  }
}

/////////////////////////////////////////////////////////////////////
// Sort indices according to descending order.
// Esta função só roda quando chamada.
void mmaxim_sort_indices_descend(  int16_t  *pn_x, int16_t *pn_indx, int16_t n_size){
  int16_t i, j, n_temp;
  for (i = 1; i < n_size; i++) {
    n_temp = pn_indx[i];
    for (j = i; j > 0 && pn_x[n_temp] > pn_x[pn_indx[j-1]]; j--)
      pn_indx[j] = pn_indx[j-1];
    pn_indx[j] = n_temp;
  }
}

/////////////////////////////////////////////////////////////////////
// Remove peaks separated by less than n_min_distance.
// Esta função só roda quando chamada.
void mmaxim_remove_close_peaks(int16_t *pn_locs, int16_t *pn_npks, int16_t *pn_x, int16_t n_min_distance){
  int16_t i, j, n_old_npks, n_dist;
    
  /* Order peaks from large to small */
  mmaxim_sort_indices_descend( pn_x, pn_locs, *pn_npks );

  for ( i = -1; i < *pn_npks; i++ ){
    n_old_npks = *pn_npks;
    *pn_npks = i+1;
    for ( j = i+1; j < n_old_npks; j++ ){
      n_dist =  pn_locs[j] - ( i == -1 ? -1 : pn_locs[i] ); // lag-zero peak of autocorr is at index -1
      if ( n_dist > n_min_distance || n_dist < -n_min_distance )
        pn_locs[(*pn_npks)++] = pn_locs[j];
    }
  }

  // Resort indices int16_to ascending order
  mmaxim_sort_ascend( pn_locs, *pn_npks );
}

/////////////////////////////////////////////////////////////////////
// Find valleys below n_min_height.
// Esta função só roda quando chamada.
void valleys_below_min_height( int16_t *pn_locs, int16_t *n_npks,  int16_t  *pn_x, int16_t n_size, int16_t n_min_height ){
  int16_t i = 1, n_width;
  *n_npks = 0;
  
  while (i < n_size-1){
    if (pn_x[i] < n_min_height && pn_x[i] < pn_x[i-1]){      // find left edge of potential valleys
      n_width = 1;
      while (i+n_width < n_size && pn_x[i] == pn_x[i+n_width])  // find flat valleys
        n_width++;
      if (pn_x[i] < pn_x[i+n_width] && (*n_npks) < 15 ){      // find right edge of valleys
        pn_locs[(*n_npks)++] = i;    
        // for flat peaks, peak location is left edge
        i += n_width+1;
      }
      else
        i += n_width;
    }
    else
      i++;
  }
}

/////////////////////////////////////////////////////////////////////
// Find valleys.
// Esta função só roda quando chamada.
void find_valleys( int16_t *pn_locs, int16_t *n_npks,  int16_t  *pn_x, int16_t n_size, int16_t n_min_height, int16_t n_min_distance, int16_t n_max_num ){
  valleys_below_min_height( pn_locs, n_npks, pn_x, n_size, n_min_height );
  mmaxim_remove_close_peaks( pn_locs, n_npks, pn_x, n_min_distance );
  *n_npks = min( *n_npks, n_max_num );
}

/////////////////////////////////////////////////////////////////////
// SPO2 calculation.
// Esta função só roda quando chamada.
void oxygen_saturation(uint16_t *pun_ir_buffer, int16_t n_ir_buffer_length, uint16_t *pun_red_buffer, int16_t *pn_spo2){
  int16_t k;
  int16_t n_i_ratio_count;
  int16_t n_exact_ir_valley_locs_count, i, n_middle_idx;
  int16_t n_th1;
  int16_t an_ir_valley_locs[15] ;
  int16_t n_y_ac, n_x_ac;
  int16_t n_y_dc_max, n_x_dc_max; 
  int16_t n_y_dc_max_idx, n_x_dc_max_idx; 
  int16_t an_ratio[5], n_ratio_average; 
  int16_t n_nume, n_denom ;

  // find precise min near an_ir_valley_locs
  find_valleys( an_ir_valley_locs, &n_exact_ir_valley_locs_count, pun_ir_buffer, BUFFER_SIZE, n_th1, 4, 15 );//peak_height, peak_distance, max_num_peaks 
  
  //using exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration an_ratio
  //finding AC/DC maximum of raw

  n_ratio_average =0; 
  n_i_ratio_count = 0; 
  for(k=0; k< 5; k++) an_ratio[k]=0;
  for (k=0; k< n_exact_ir_valley_locs_count; k++){
    if (an_ir_valley_locs[k] > BUFFER_SIZE ){
      *pn_spo2 =  -999 ; // do not use SPO2 since valley loc is out of range
      return;
    }
  }
  
  // find max between two valley locations 
  // and use an_ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2 
  for (k=0; k< n_exact_ir_valley_locs_count-1; k++){
    n_y_dc_max= -16777216 ; 
    n_x_dc_max= -16777216; 
    if (an_ir_valley_locs[k+1]-an_ir_valley_locs[k] >3){
        for (i=an_ir_valley_locs[k]; i< an_ir_valley_locs[k+1]; i++){
          if (pun_ir_buffer[i]> n_x_dc_max) {n_x_dc_max =pun_ir_buffer[i]; n_x_dc_max_idx=i;}
          if (pun_red_buffer[i]> n_y_dc_max) {n_y_dc_max =pun_red_buffer[i]; n_y_dc_max_idx=i;}
      }
      n_y_ac= (pun_red_buffer[an_ir_valley_locs[k+1]] - pun_red_buffer[an_ir_valley_locs[k] ] )*(n_y_dc_max_idx -an_ir_valley_locs[k]); //red
      n_y_ac=  pun_red_buffer[an_ir_valley_locs[k]] + n_y_ac/ (an_ir_valley_locs[k+1] - an_ir_valley_locs[k])  ; 
      n_y_ac=  pun_red_buffer[n_y_dc_max_idx] - n_y_ac;    // subracting linear DC compoenents from raw 
      n_x_ac= (pun_ir_buffer[an_ir_valley_locs[k+1]] - pun_ir_buffer[an_ir_valley_locs[k] ] )*(n_x_dc_max_idx -an_ir_valley_locs[k]); // ir
      n_x_ac=  pun_ir_buffer[an_ir_valley_locs[k]] + n_x_ac/ (an_ir_valley_locs[k+1] - an_ir_valley_locs[k]); 
      n_x_ac=  pun_ir_buffer[n_y_dc_max_idx] - n_x_ac;      // subracting linear DC compoenents from raw 
      n_nume=( n_y_ac *n_x_dc_max)>>7 ; //prepare X100 to preserve floating value
      n_denom= ( n_x_ac *n_y_dc_max)>>7;
      if (n_denom>0  && n_i_ratio_count <5 &&  n_nume != 0){   
        an_ratio[n_i_ratio_count]= (n_nume*100)/n_denom ; //formular is ( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max) ;
        n_i_ratio_count++;
      }
    }
  }
  
  // choose median value since PPG signal may varies from beat to beat
  mmaxim_sort_ascend(an_ratio, n_i_ratio_count);
  n_middle_idx= n_i_ratio_count/2;

  if (n_middle_idx >1)
    n_ratio_average =( an_ratio[n_middle_idx-1] +an_ratio[n_middle_idx])/2; // use median
  else
    n_ratio_average = an_ratio[n_middle_idx ];
      
  if( n_ratio_average>2 && n_ratio_average <184){
    float float_SPO2 =  -45.060*(float)n_ratio_average*(float)n_ratio_average/10000.0 + 30.354*(float)n_ratio_average/100.0 + 94.845;
    *pn_spo2 = float_SPO2 ;
  }
  else{
    *pn_spo2 =  -999 ; // do not use SPO2 since signal an_ratio is out of range
  }  
}

/////////////////////////////////////////////////////////////////////
// Chama as funções necessárias para o cálculo do SpO2.
// Esta função só roda quando chamada.
int16_t medeSpO2(){
    int16_t SpO2_out = -1;
    // calcula SpO2 e frequência cardíaca
    int16_t spo2_temp;
    oxygen_saturation(irBuffer, BUFFER_LENGTH, redBuffer, &spo2_temp);
    if(spo2_temp>0) SpO2_out = spo2_temp; // se valor < 0, a medida é inválida
    return SpO2_out;
}
