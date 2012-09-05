<?php
/**
 * @file
 * Layout for advanced A4
 */
class ExerciseAdvancedPdfPortrait extends TCPDF
{
  function __construct($disccache = FALSE)
  {
    parent::__construct('P','mm','A4', TRUE, 'UTF-8', $disccache);
    $this->SetAutoPageBreak(FALSE);
    $this->SetMargins(0, 0, 0);  
    $this->AliasNbPages();    
  }

  function AddPage($node)
  {
    global $base_url;
    parent::AddPage();
    $this->SetMargins(0, 0, 0);  
    $this->setX(0);
    $this->setY(0);    
    $title = "  " . $node->title;
    $intro = $node->field_exercise_intro[0]['value'];
    $description = $node->field_exercise_guide[0]['value'];

    $keywords = array();
    foreach ($node->taxonomy as $taxonomy) {
      $keywords[] = $taxonomy->name;
    }

    $url = $base_url. '/node/' . $node->nid;
  
    $title_size = 20;

    $this->SetFont('Helvetica', 'B', $title_size);
    $this->SetTextColor(255, 255, 255);
    $this->Cell(0, 20, $title, null, 2, 'L', TRUE);
  
    $this->SetLeftMargin(10);
    $this->SetRightMargin(10);
    
    $this->SetY(230);
    $this->SetX(10);
    $this->SetTextColor(0, 0, 0);

    $this->SetFont('Helvetica', null, 10);
    $this->MultiCell(40, 5, implode($keywords, "\n"), 0, 'C');

    if (!empty($node->field_exercise_images[0])) {
      $x = 10;
      $y = 25;
      $width = 0;
      $spacing = 3;
      $count = 0;
      $picture_rows = 1;
      
      $pr_row = 0;
      foreach ($node->field_exercise_images as $image) {
        $picture_filename = drupal_realpath(image_style_path('square_thumbnail', $image['uri']));
        
        if (file_exists($picture_filename)) {
          $file = $picture_filename;
          $size = getimagesize($file);
          if ($size[0] < $size[1]) {
            $orientation = 'portrait';
            $pic_width = 25;
            $new_line = 45;
            $no_pr_row = 2;
            if ($count > 8) {
              break;
            }
          }
          else {
            $orientation = 'landscape';
            $pic_width = 55;
            $new_line = 40;
            $no_pr_row = 1;            
            if ($count > 5) {
              break;
            }
          }
          $width += $pic_width + $spacing;
    
          if ($pr_row >= $no_pr_row) {
            $y += $new_line;
            $x = 10;
            $picture_rows++;
            $width = 0;
            $pr_row = 0;
          }
          
          $this->Image($file, $x, $y, $pic_width, 0, '');
          $x += $pic_width + $spacing;
          $pr_row++;          
        }
      }
    }
  
    $this->SetFont('Helvetica', 'B', 12);
    $this->setTextColor(0, 0, 0);
    $this->setY(30);
    $this->setX(70);
    $this->MultiCell(0, 7, $intro, 0, 'L');
    $this->Ln(4);
    $this->SetFont('Helvetica', null, 12);
    $this->setX(70);
    $this->MultiCell(0, 6, str_replace("\n",' ',$description), 0, 'L', FALSE, 1, '', '', TRUE, 0, TRUE);

    $this->Image(dirname(__FILE__) . '/../../../mp-logo.png', 8, 270, 50, 0, '', 'http://motionsplan.dk/');
    //$this->Image(dirname(__FILE__) . '/vih_logo.jpg',80,268,45,0,'', 'http://motionsplan.dk/');    
    $this->Image(dirname(__FILE__) . '/../../../cc-by-sa_340x340.png', 190, 3, 17, 0, '');

    $this->SetFont('Helvetica', null, 8);
    $this->setY(280);
    $this->setX(6);
    $this->MultiCell(50, 8, $url, 0, 'C');

    $image_url = "http://chart.apis.google.com/chart?cht=qr&chs=100x100&chl=" . $url;
  
    $ch = curl_init();
    $timeout = 0;
    curl_setopt ($ch, CURLOPT_URL, $image_url);
    curl_setopt ($ch, CURLOPT_CONNECTTIMEOUT, $timeout);

    // Getting binary data
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);

    $image = curl_exec($ch);

    curl_close($ch);

    $f = fopen(getcwd() . '/' . file_directory_path() . '/barcodes/'. md5(request_uri()) . '.png', 'w');
    fwrite($f, $image);
    fclose($f);

    $this->Image(getcwd() . '/' . file_directory_path() . '/barcodes/' . md5(request_uri()) . '.png', 160, 245, 45, 0, '');
  }
}

