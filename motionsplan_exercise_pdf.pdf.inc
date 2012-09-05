<?php
/**
* @file
* Code for the PDF's
*/

class ExerciseProgramCompactPdf extends FPDF {
  var $widths;
  var $aligns;
  var $imgs;

  /**
   * Set the array of column width
   */
  function SetWidths($w) {
    $this->widths=$w;
  }

  /**
   * Set the array of column alignments
   */
  function SetAligns($a) {
    $this->aligns=$a;
  }

  function Row($data, $pics = array()) {
    //Calculate the height of the row
    $pic_width = 15;
    $pic_height = 15;
    $nb=0;
    for($i=0;$i<count($data);$i++) {
      $nb=max($nb,$this->NbLines($this->widths[$i],$data[$i]));
    }
    $h=5*$nb;

    if ($first_col = $this->NbLines($this->widths[$i], $data[0]) * 5 + $pic_height >= $h AND !empty($pics)) {
      $h = $first_col * 5 + $pic_height + 6;
    }

    //Issue a page break first if needed
    $this->CheckPageBreak($h);
    //Draw the cells of the row
    for($i = 0; $i < count($data); $i++) {
      $w=$this->widths[$i];
      $a=isset($this->aligns[$i]) ? $this->aligns[$i] : 'L';
      //Save the current position
      $x=$this->GetX();
      $y=$this->GetY();
      //Draw the border
      $this->Rect($x, $y, $w, $h);
      //Print the text
      $this->MultiCell($w, 5, $data[$i], 0, $a);

      $calc_x = $x + 1;
      $calc_y = $this->GetY();
      if (!empty($pics)) {
        if ($i==0) {
          foreach ($pics as $pic) {
            if (file_exists($pic)) {
              $this->Image($pic, $calc_x, $calc_y, $pic_width, $pic_height);
              $calc_x += $pic_width;
            }
          }  
        }
      }
      //Put the position to the right of the cell
      $this->SetXY($x+$w,$y);
    }

    //Go to the next line
    $this->Ln($h);
  }

  /**
   * If the height h would cause an overflow, add a new page immediately
   *
   * @param integer $h Height
   *
   * @return void
   */
  function CheckPageBreak($h) {
    if ($this->GetY() + $h > $this->PageBreakTrigger) {
      $this->AddPage($this->CurOrientation);
    }
  }
  
  /**
   * Computes the number of lines a MultiCell of width w will take
   *
   * @param integer $w   Width
   * @param string  $txt Text
   */
  function NbLines($w, $txt) {
    $cw=&$this->CurrentFont['cw'];
    if ($w==0) {
      $w=$this->w-$this->rMargin-$this->x;
    }
    $wmax=($w-2*$this->cMargin)*1000/$this->FontSize;
    $s=str_replace("\r",'',$txt);
    $nb=strlen($s);
    if ($nb>0 and $s[$nb-1]=="\n") {
      $nb--;
    }
    $sep=-1;
    $i=0;
    $j=0;
    $l=0;
    $nl=1;
    while($i<$nb) {
      $c=$s[$i];
      if ($c=="\n") {
        $i++;
        $sep=-1;
        $j=$i;
        $l=0;
        $nl++;
        continue;
      }
      if ($c==' ') {
        $sep=$i;
      }
      $l+=$cw[$c];
      if ($l>$wmax) {
        if ($sep==-1) {
          if ($i==$j) {
            $i++;
          }
        }
        else {
          $i=$sep+1;
        }
        $sep=-1;
        $j=$i;
        $l=0;
        $nl++;
      }
      else {
        $i++;
      }
    }
    return $nl;
  }
}

class ArticlePdf extends TCPDF {
  protected $node;
  protected $url;

  function __construct($node) {
    global $base_url;      
    parent::__construct('P','mm','A4');
    $this->SetAutoPageBreak(TRUE, 30);
    $this->SetMargins(10, 10, 10);  
    $this->AliasNbPages();    
    $this->node = $node;
    
    $this->url = $base_url. '/node/' . $this->node->nid;    
  }
  
  function Header() {
  }
  
  function Footer() {
    $this->Image(dirname(__FILE__) . '/mp-logo.png',8,275,50,0,'', 'http://motionsplan.dk/');
    $this->Image(dirname(__FILE__) . '/vih_logo.jpg',155,270,45,0,'', 'http://vih.dk/');      
    $this->SetFont('Helvetica', null, 8);
    $this->setY(285);
    $this->setX(6);
    $this->MultiCell(50, 8, $this->url, 0, 'C');
    $this->SetY(-15);
    $this->SetFont('Helvetica', null, 12);    
    $this->Cell(0,10, $this->PageNo().'/{nb}',0,0,'C');
  }

  function AddContent() {
    parent::AddPage();
    $title = '  ' . $this->node->title;
    $body = check_markup($this->node->body, $this->node->format, FALSE);

    $keywords = array();
    if (!empty($this->node->taxonomy)) {
      foreach ($this->node->taxonomy as $taxonomy) {
        $keywords[] = $taxonomy->name;
      }
    }

    if (!empty($this->node->field_article_image[0]['filepath'])) {
      $presetname = 'SlideshowPicture';
      $preset = imagecache_preset_by_name($presetname);
      $src = $this->node->field_article_image[0]['filepath'];
      $dst = imagecache_create_path($presetname, $src);
      if (file_exists($dst) || imagecache_build_derivative($preset['actions'], $src, $dst)) {
        $this->Image($dst,10,10,190,0,'', 'http://motionsplan.dk/');
      }    
    }
    $this->Image(dirname(__FILE__) . '/cc-by-sa_340x340.png', 180, 13, 17, 0, '');
  
    $this->SetY(43);
    $title_size = 15;
    $this->SetFont('Helvetica', 'B', $title_size);
    $this->SetTextColor(255, 255, 255);
    $this->Cell(0, 17, $title, null, 2, 'L', TRUE);
  
    $this->Ln(8);
    $this->SetFont('Helvetica', 'N', 10);
    $this->SetTextColor(0, 0, 0);
    $this->writeHTML($body, TRUE, FALSE, TRUE, FALSE, '');    
  }
}

