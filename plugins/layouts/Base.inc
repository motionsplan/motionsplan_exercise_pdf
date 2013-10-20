<?php
require_once libraries_get_path('tcpdf') . '/tcpdf.php';

abstract class Motionsplan_Pdf extends TCPDF {
  
  /**
   * Gets barcode file path
   *
   * @param string  $url url
   * @param integer $height Height
   * @param integer $width Width
   *
   * @return string or false
   */
  protected function getBarcodePath($url, $height, $width) {
    $chart = array(
      '#chart_id' => 'exercise_chart',
      '#type' => CHART_TYPE_QR,
      '#size' => chart_size(200, 200) 
    );
        
    $chart['#data'][] = '';
    $chart['#labels'][] = $url;
        
    return chart_copy($chart, 'exercise_chart_' . uniqid(), 'public://charts/');
  }  
}