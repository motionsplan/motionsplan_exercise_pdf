<?php
/**
 * @file
 * Layout for default A4
 */
class ExercisePdfPortrait extends FPDF
{
  function __construct() {
    parent::fpdf('P','mm','A4');
    $this->SetAutoPageBreak(FALSE);
    $this->SetMargins(0, 0, 0);  
  }

  function AddPage($node) {
    global $base_url;
    parent::AddPage();
    $this->SetMargins(0, 0, 0);  
    $this->setX(0);
    $this->setY(0);    
    $title = "  " . utf8_decode($node->title);
    $description = utf8_decode(strip_tags($node->field_exercise_intro[LANGUAGE_NONE][0]['value']));
    $keywords = array();
    foreach ($node->taxonomy as $taxonomy) {
      $keywords[] = $taxonomy->name;
    }

    $url = $base_url. '/node/' . $node->nid;
  
    $title_size = 30;

    $this->SetFont('Helvetica', 'B', $title_size);

    $title_width = $this->GetStringWidth($title);
  
    if ($title_width > 200) {
      $title_size = 25;
    }
  
    $this->SetFont('Helvetica', 'B', $title_size);
    $this->SetTextColor(255, 255, 255);
    $this->Cell(0, 50, $title, null, 2, 'L', TRUE);
  
    $this->SetLeftMargin(10);
    $this->SetRightMargin(10);
    $this->SetY(35);
    $this->SetFont('Helvetica', null, 10);
    $this->MultiCell(0, 5, utf8_decode(implode($keywords, ", ")), 0, 'L');

    $this->setY(60);

    if (!empty($node->field_exercise_images[LANGUAGE_NONE][0])) {
      $x = 10;
      $y = 60;
      $width = 0;
      $spacing = 5;
      $count = 0;
      $picture_rows = 1;
      $style = 'exercise_extended_pdf';
      $style_array = image_style_load($style);
      $no_of_pics = count($node->field_exercise_images[LANGUAGE_NONE]);
      foreach ($node->field_exercise_images[LANGUAGE_NONE] as $image) {
        $dst = image_style_path($style, $image['uri']);
        if (file_exists($dst) || image_style_create_derivative($style_array, $image['uri'], $dst)) {
          $file = image_style_path($style, $image['uri']);
          $size = getimagesize($file);
          if ($size[0] < $size[1]) {
            $orientation = 'portrait';
            if ($no_of_pics <= 6) {
              $pic_width = 55;
              $new_line = 80;
            }
            elseif ($no_of_pics > 6 && $no_of_pics < 9) {
              $pic_width = 40;
              $new_line = 60;
            }
            elseif ($no_of_pics > 8) {
              $pic_width = 30;
              $new_line = 50;
            }
          }
          else {
            $orientation = 'landscape';
            $pic_width = 80;
            $new_line = 68;
          }
          $width += $pic_width + $spacing;
    
          if ($width > 200) {
            $y += $new_line;
            $x = 10;
            $picture_rows++;
            $width = 0;
          }
          
          $this->Image($file, $x, $y, $pic_width, 0, '');
          $x += $pic_width + $spacing;
        }
      }
      if ($orientation == 'portrait') {
        if ($picture_rows == 1) {
          $this->setY(150);
        }
        else {
          $this->setY(230);
        }
      }
      else {
        if ($picture_rows == 1) {
          $this->setY(125);
        }
        else {
          $this->setY(195);
        }
      }      
    }
  
    $this->SetFont('Helvetica', null, 17);
    $this->setTextColor(0, 0, 0);

    $this->MultiCell(0, 8, $description, 0);

    $this->Image(dirname(__FILE__) . '/../../../mp-logo.png', 8, 270, 50, 0, '', 'http://motionsplan.dk/');
    $this->Image(dirname(__FILE__) . '/../../../vih_logo.jpg', 80, 268, 45, 0, '', 'http://motionsplan.dk/');    
    $this->Image(dirname(__FILE__) . '/../../../cc-by-sa_340x340.png', 190, 3, 17, 0, '');

    $this->SetFont('Helvetica', null, 8);
    $this->setY(280);
    $this->setX(6);
    $this->MultiCell(50, 8, $url, 0, 'C');

    $qr_file = $this->getBarcodePath($url, 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
      $this->Image($qr_file,  160, 245, 45, 0, '');
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

