<?php
/**
 * @file
 * motionsplan_exercise_pdf.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function motionsplan_exercise_pdf_image_default_styles() {
  $styles = array();

  // Exported image style: exercise_extended_pdf.
  $styles['exercise_extended_pdf'] = array(
    'name' => 'exercise_extended_pdf',
    'label' => 'exercise_extended_pdf',
    'effects' => array(
      3 => array(
        'label' => 'Skalér',
        'help' => 'Skalering bevarer billedets højde-bredde-forhold. Hvis kun den ene dimension angives vil den anden blive beregnet.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 250,
          'height' => 250,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: exercise_pdf.
  $styles['exercise_pdf'] = array(
    'name' => 'exercise_pdf',
    'label' => 'exercise_pdf',
    'effects' => array(
      4 => array(
        'label' => 'Skalér',
        'help' => 'Skalering bevarer billedets højde-bredde-forhold. Hvis kun den ene dimension angives vil den anden blive beregnet.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 600,
          'height' => 600,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
