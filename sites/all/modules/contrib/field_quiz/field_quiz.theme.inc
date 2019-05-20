<?php
/**
 * @file
 * Theming functions for the field for creating a single multiple choice question with answers in a entity.
 */


/**
 * Theme the sorry, wrong answer page
 */
function theme_field_quiz_page_wrong_answer($vars) {
  $content = '';
  $content .= '<div class="field-quiz-answer-wrong">';
  // If not ajax, so the answer is displayed on a separate page.
  if (empty($vars['ajax'])) {
    $content .= $vars['text'];
    $content .= "<br />" . PHP_EOL;
    $content .= l($vars['back_link_text'], str_replace('_', '/', $vars['entity_type']) . '/' . $vars['entity_id']);
  }
  else {
    // If ajax and the answer is displayed below the answer options.
    $content .= $vars['text'];
  }
  $content .= '</div>';
  return $content;
}


/**
 * correct answer page
 */
function theme_field_quiz_page_correct_answer($vars) {
  $content = '';
  $content .= '<div class="field-quiz-answer-correct">';
  // If not ajax, so the answer is displayed on a separate page.
  if (empty($vars['ajax'])) {
    $content .= $vars['text'];
    $content .= "<br />" . PHP_EOL;
    $content .= l($vars['back_link_text'], str_replace('_', '/', $vars['entity_type']) . '/' . $vars['entity_id']);
  }
  else {
    // If ajax and the answer is displayed below the answer options.
    $content .= $vars['text'];
  }
  $content .= '</div>';
  return $content;
}

