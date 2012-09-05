<?php
/**
 * @file
 * Layout for default A4
 */
class ExercisePdfPortrait extends FPDF
{
    function __construct()
    {
        parent::fpdf('P','mm','A4');
        $this->SetAutoPageBreak(FALSE);
        $this->SetMargins(0, 0, 0);  
    }

    function AddPage($node)
    {
        global $base_url;
        parent::AddPage();
        $this->SetMargins(0, 0, 0);  
        $this->setX(0);
        $this->setY(0);        
        $title = "  " . utf8_decode($node->title);
        $description = utf8_decode(strip_tags($node->field_exercise_intro[0]['value']));
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

        if (!empty($node->field_exercise_images[0])) {
            $x = 10;
            $y = 60;
            $width = 0;
            $spacing = 5;
            $count = 0;
            $picture_rows = 1;
            $presetname = 'exercisePdf';
            $preset = imagecache_preset_by_name($presetname);
            foreach ($node->field_exercise_images as $image) {
                $src = $image["filepath"];
                $dst = imagecache_create_path($presetname, $src);
                $count++;                
                if (file_exists($dst) || imagecache_build_derivative($preset['actions'], $src, $dst)) {
                    $file = $dst;
                    $size = getimagesize($file);
                    if ($size[0] < $size[1]) {
                        $orientation = 'portrait';
                        $pic_width = 55;
                        $new_line = 80;
                        if ($count > 6) {
                            break;
                        }
                    } else {
                        $orientation = 'landscape';
                        $pic_width = 80;
                        $new_line = 68;
                        if ($count > 4) {
                            break;
                        }
                    }
                    $width += $pic_width + $spacing;
        
                    if ($width > 200) {
                        $y += $new_line;
                        $x = 10;
                        $picture_rows++;
                        $width = 0;
                    }
                    
                    $this->Image($file,$x,$y,$pic_width,0,'');
                    $x += $pic_width + $spacing;

                }
            }
            if ($orientation == 'portrait') {
                if ($picture_rows == 1) {
                    $this->setY(150);
                } else {
                    $this->setY(230);
                }
            } else {
                if ($picture_rows == 1) {
                    $this->setY(125);
                } else {
                    $this->setY(195);
                }
            }            
        }
    
        $this->SetFont('Helvetica', null, 17);
        $this->setTextColor(0, 0, 0);

        $this->MultiCell(0, 8, $description, 0);

        $this->Image(dirname(__FILE__) . '/mp-logo.png',8,270,50,0,'', 'http://motionsplan.dk/');
        $this->Image(dirname(__FILE__) . '/vih_logo.jpg',80,268,45,0,'', 'http://motionsplan.dk/');        
        $this->Image(dirname(__FILE__) . '/cc-by-sa_340x340.png',190,3,17,0,'');

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

