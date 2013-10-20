<?php
/**
 * @file
 * Layout in landscape for an exercise
 */

require_once dirname(__FILE__) . '/../Base.inc';

class ExercisePdfLandscape extends Motionsplan_Pdf {
  function __construct($disccache = FALSE) {
    parent::__construct('L','mm','A4', TRUE, 'UTF-8', $disccache);
    $this->SetAutoPageBreak(FALSE);
    $this->AliasNbPages();    
  }
  function Header() {
    // intentionally left blank to avoid line in the header
  }

  function AddPage($node) {
    global $base_url;

    parent::AddPage();

    $title = $node->title;
    $description = strip_tags($node->field_exercise_intro[LANGUAGE_NONE][0]['value']);
    $keywords = array();
    foreach ($node->taxonomy as $taxonomy) {
      $keywords[] = $taxonomy->name;
    }

    $url = $base_url. '/node/' . $node->nid;

    $this->SetFont('Helvetica', 'B', 26);
    $this->setTextColor(255, 255, 255);
    $this->Cell(0, 22, $title, null, 2, 'C', TRUE);

    $this->SetFont('Helvetica', null, 17);
    $this->setTextColor(0, 0, 0);
    $this->setY(40);

    $this->MultiCell(200, 8, str_replace("\n",' ',$description), 0, 'L', FALSE, 1, '', '', TRUE, 0, TRUE);

    if (!empty($node->field_exercise_images[LANGUAGE_NONE][0])) {
      $x = 10;
      $y = 80;
      $width = 0;
      $spacing = 5;
      $count = 0;
      $style = 'exercise_extended_pdf';
      $style_array = image_style_load($style);
      $no_of_pics = count($node->field_exercise_images[LANGUAGE_NONE]);
      foreach ($node->field_exercise_images[LANGUAGE_NONE] as $image) {
        $dst = image_style_path($style, $image['uri']);
        if (file_exists($dst) || image_style_create_derivative($style_array, $image['uri'], $dst)) {
          $file = image_style_path($style, $image['uri']);
          $count++;
          $size = getimagesize($file);
          if ($size[0] < $size[1]) {
            // portrait
            $pic_width = 35;
            $new_line = 60;
            if ($count > 10) {
              break;
            }
          }
          else {
            // landscape
            $pic_width = 55;
            $new_line = 50;
            if ($count > 6) {
              break;
            }
          }
          $width += $pic_width + $spacing;
  
          if ($width > 200) {
            $y += $new_line;
            $x = 10;
          }
  
          $this->Image($file,$x,$y,$pic_width,0,'');
          $x += $pic_width + $spacing;
        }      
      
      }
    }
  
    $this->Line(220, 30, 220, 220);
  
    $this->Image(dirname(__FILE__) . '/../../../vih_logo.jpg',240,120,32,0,'', 'http://vih.dk/');
    $this->Image(dirname(__FILE__) . '/../../../mp-logo.png',235,180,50,0,'', 'http://motionsplan.dk/');

    $this->Image(dirname(__FILE__) . '/../../../cc-by-sa_340x340.png',12,12,17,0,'');

    $this->setY(168);
    $this->SetFont('Helvetica', null, 10);

    $this->setY(40);
    $this->setX(233);

    $this->MultiCell(40, 5, implode($keywords, "\n"), 0, 'C');

    $this->SetFont('Helvetica', null, 8);
    $this->setY(190);
    $this->setX(230);
    $this->MultiCell(60, 8, $url, 0, 'C');

    $qr_file = $this->getBarcodePath($url, 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
       $this->Image($qr_file, 235, 135, 45, 0, '');
    }
  }

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
