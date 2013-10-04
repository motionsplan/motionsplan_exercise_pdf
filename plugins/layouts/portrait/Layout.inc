<?php
/**
 * @file
 * Layout for default A4
 */
class ExercisePdfPortrait extends TCPDF {
  protected $font = 'Helvetica';
  protected $title_fill_color = array(0, 0, 0);

  public function __construct($disccache = FALSE) {
    parent::__construct('P','mm','A4', TRUE, 'UTF-8', $disccache);
    $this->SetAutoPageBreak(FALSE);
    $this->SetMargins(0, 0, 0);  
    $this->AliasNbPages();    
  }
  
  protected function getPictureFilename() {
    
  }
  
  /**
   * Set title color in RGB array
   *
   * @var array $rgb An array with rgb codes
   *
   * @return void
   */
  public function setTitleBackgroundColor(array $rgb) {
    $this->title_fill_color = $rgb;
  }
  
  protected function addTitle($title) {
    $title_size = 30;
    $title_width = $this->GetStringWidth($title);
    if ($title_width > 200) {
      $title_size = 25;
    }
    $this->SetFont($this->font, 'B', $title_size);
    $this->SetTextColor(255, 255, 255);
    $this->SetFillColorArray($this->title_fill_color);
    $this->Cell(0, 30, $title, null, 2, 'L', TRUE);  
  } 

  public function AddPage($node) {
    global $base_url;
    parent::AddPage();
    $this->SetMargins(0, 0, 0);  
    $this->setX(0);
    $this->setY(0);    
    $url = $base_url. '/node/' . $node->nid;
    $this->addTitle("  " . $node->title);
    $description = strip_tags($node->field_exercise_intro[LANGUAGE_NONE][0]['value']);
    $keywords = array();
    /*
    foreach ($node->taxonomy as $taxonomy) {
      $keywords[] = $taxonomy->name;
    }
    */


    $this->SetLeftMargin(10);
    $this->SetRightMargin(10);
    /*
    $this->SetY(35);
    $this->SetFont($this->font, null, 10);
    $this->MultiCell(0, 5, utf8_decode(implode($keywords, ", ")), 0, 'L');
    */
    $this->setY(40);

    if (!empty($node->field_exercise_images[LANGUAGE_NONE][0])) {
      $x = 10;
      $y = $this->GetY();
      $new_y = $y;
      $width = 0;
      $spacing = 5;
      $count = 0;
      $picture_rows = 1;
      $style = 'exercise_pdf';
      $style_array = image_style_load($style);
      $no_of_pics = count($node->field_exercise_images[LANGUAGE_NONE]);
      foreach ($node->field_exercise_images[LANGUAGE_NONE] as $image) {
        $dst = image_style_path($style, $image['uri']);
        if (file_exists($dst) || image_style_create_derivative($style_array, $image['uri'], $dst)) {
          $file = image_style_path($style, $image['uri']);
          $size = getimagesize($file);
          if ($size[0] < $size[1]) {
            $orientation = 'portrait';
            if ($no_of_pics <= 2) {
              $pic_width = 93;
              $new_line = 145;
            }
            elseif ($no_of_pics <= 3) {
              $pic_width = 60;
              $new_line = 95;
            }
            elseif ($no_of_pics <= 8) {
              $pic_width = 45;
              $new_line = 72;
            }
            else {
              $pic_width = 34;
              $new_line = 55;
            }
          }
          else {
            $orientation = 'landscape';
            if ($no_of_pics == 1) {
              $pic_width = 190;
              $new_line = 130;
            }
            elseif ($no_of_pics <= 4) {
              $pic_width = 92;
              $new_line = 66;
            }
            elseif ($no_of_pics <= 9) {
              $pic_width = 60;
              $new_line = 44;
            }
            else {
              $pic_width = 40;
              $new_line = 34;
            }
          }
          $width += $pic_width + $spacing;
    
          if ($width > 200) {
            $y += $new_line;
            $x = 10;
            $picture_rows++;
            $width = 0;
            $new_y += $new_line;
          }
          
          $this->Image($file, $x, $y, $pic_width, 0, '');
          $x += $pic_width + $spacing;
        }
      }
      $this->setY($new_y + $new_line);
    }
  
    $this->SetFont($this->font, null, 17);
    $this->setTextColor(0, 0, 0);
    $this->MultiCell(0, 8, str_replace("\n", ' ', $description), 0, 'L', FALSE, 1, '', '', TRUE, 0, TRUE);
    //$this->MultiCell(0, 8, $description, 0);

    $this->Image(dirname(__FILE__) . '/../../../mp-logo.png', 8, 270, 50, 0, '', 'http://motionsplan.dk/');
    $this->Image(dirname(__FILE__) . '/../../../vih_logo.jpg', 80, 268, 45, 0, '', 'http://motionsplan.dk/');

    $this->SetFont($this->font, null, 8);
    $this->setY(280);
    $this->setX(6);
    $this->MultiCell(50, 8, $url, 0, 'C');

    $qr_file = $this->getBarcodePath($url, 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
      $this->Image($qr_file, 182, 3, 24, 0);
    }
    
    $this->SetLineWidth(1.6);
    $this->SetDrawColor(185);
    $this->Circle(190, 292, 45);
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

