<?php
/**
 * @file
 * Layout for advanced A4
 */

require_once dirname(__FILE__) . '/../Base.inc';

class ExerciseAdvancedPdfPortrait extends Motionsplan_Pdf {
  function __construct($disccache = FALSE) {
    parent::__construct('P','mm','A4', TRUE, 'UTF-8', $disccache);
    $this->SetAutoPageBreak(FALSE);
    $this->SetMargins(0, 0, 0);
    $this->AliasNbPages();
  }

  function AddNewPage($node) {
    global $base_url;
    parent::AddPage();
    $this->SetMargins(0, 0, 0);
    $this->setX(0);
    $this->setY(0);
    $title = "  " . $node->title;
    $intro = $node->field_exercise_intro[LANGUAGE_NONE][0]['value'];
    $description = $node->field_exercise_guide[LANGUAGE_NONE][0]['value'];

    $keywords = array();
    /*
    foreach ($node->taxonomy as $taxonomy) {
      $keywords[] = $taxonomy->name;
    }
    */

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

    // Image handling.
    $old_orientation = '';
    $style = 'exercise_extended_pdf';
    $style_array = image_style_load($style);

    if (!empty($node->field_exercise_images[LANGUAGE_NONE][0])) {
      $x = 10;
      $y = 25;
      $width = 0;
      $spacing = 3;
      $count = 0;
      $picture_rows = 1;
      $pr_row = 0;
      $no_of_pics = count($node->field_exercise_images[LANGUAGE_NONE]);
      foreach ($node->field_exercise_images[LANGUAGE_NONE] as $image) {
        $dst = image_style_path($style, $image['uri']);

        if (file_exists($dst) || image_style_create_derivative($style_array, $image['uri'], $dst)) {
          $file = image_style_path($style, $image['uri']);
          $size = getimagesize($file);
          if ($size[0] < $size[1]) {
            $orientation = 'portrait';
            if ($no_of_pics <= 2) {
              $pic_width = 55;
              $new_line = 45;
              $no_pr_row = 1;
            }
            elseif ($no_of_pics <= 3) {
              $pic_width = 50;
              $new_line = 40;
              $no_pr_row = 1;
            }
            else {
              $pic_width = 26;
              $new_line = 43;
              $no_pr_row = 2;
              if ($count > 8) {
                break;
              }
            }
          }
          else {
            $orientation = 'landscape';
            $pic_width = 55;
            $new_line = 20;
            $no_pr_row = 1;
            if ($count > 5) {
              break;
            }
          }

          if ($old_orientation != $orientation) {
            $pr_row = 0;
            if ($old_orientation == 'landscape') {
              $new_line += 40;
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

          if ($no_pr_row == 1) {
            $y += $new_line;
            $x = 10;
          }
          $pr_row++;
          $old_orientation = $orientation;
          $count++;
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

    $qr_file = $this->getBarcodePath($url, 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
      $this->Image($qr_file,  160, 245, 45, 0, '');
    }
  }
}
