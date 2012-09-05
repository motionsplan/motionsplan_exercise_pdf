<?php
/**
 * @file
 * Layout in landscape for an exercise
 */

class ExercisePdfLandscape extends FPDF
{
    function __construct()
    {
        parent::fpdf('L','mm','A4');
        $this->SetAutoPageBreak(FALSE);
    }

    function AddPage($node)
    {
        global $base_url;

        parent::AddPage();

        $title = utf8_decode($node->title);
        $description = utf8_decode(strip_tags($node->field_exercise_intro[0]['value']));
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

        $this->MultiCell(200, 8, $description, 0);

        if (!empty($node->field_exercise_images[0])) {
            $x = 10;
            $y = 80;
            $width = 0;
            $spacing = 5;
            $count = 0;
            $presetname = 'exercisePdf';
            $preset = imagecache_preset_by_name($presetname);
            foreach ($node->field_exercise_images as $image) {
                $src = $image["filepath"];
                $dst = imagecache_create_path($presetname, $src);
                if (file_exists($dst) || imagecache_build_derivative($preset['actions'], $src, $dst)) {
                    $count++;
                    $file = $dst;
                    $size = getimagesize($file);
                    if ($size[0] < $size[1]) {
                        // portrait
                        $pic_width = 35;
                        $new_line = 60;
                        if ($count > 10) {
                            break;
                        }
                    } else {
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
    
        $this->Image(dirname(__FILE__) . '/vih_logo.jpg',240,120,32,0,'', 'http://vih.dk/');
        $this->Image(dirname(__FILE__) . '/mp-logo.png',235,180,50,0,'', 'http://motionsplan.dk/');

        $this->Image(dirname(__FILE__) . '/cc-by-sa_340x340.png',12,12,17,0,'');

        $this->setY(168);
        $this->SetFont('Helvetica', null, 10);

        $this->setY(40);
        $this->setX(233);

        $this->MultiCell(40, 5, utf8_decode(implode($keywords, "\n")), 0, 'C');

        $this->SetFont('Helvetica', null, 8);
        $this->setY(190);
        $this->setX(230);
        $this->MultiCell(60, 8, $url, 0, 'C');
    
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
    
        $this->Image(getcwd() . '/' . file_directory_path() . '/barcodes/' . md5(request_uri()) . '.png', 235, 135, 45, 0, '');
    }
}
