<?php
/**
 * @file
 * Administration settings for the field quiz.
 */


/**
 * Administration settings form.
 *
 * @return array
 */
function field_quiz_admin_settings_form() {
  $form = array();

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => t('Administration settings for the field quiz module.'),
    '#suffix' => '<div>&nbsp;</div>',
  );

  // Use radios as answer type.
  $form['field_quiz_use_radios'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use radios, if just one answer is possible'),
    '#description' => t('Use radios if there is just one possible answer available.'),
    '#default_value' => variable_get('field_quiz_use_radios', FALSE),
  );

  // Do not redirect after submission.
  $form['field_quiz_use_ajax'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use ajax'),
    '#description' => t('Use ajax to display the result message. 
    If turned on the success of fail message will be displayed within the form page. 
    If turned off the user will be redirected to pages.'),
    '#default_value' => variable_get('field_quiz_use_ajax', TRUE),
  );

  // Shuffle the answers.
  $form['field_quiz_shuffle_answers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shuffle the answers'),
    '#description' => t('If checked, the order of the answers will be shuffled.'),
    '#default_value' => variable_get('field_quiz_shuffle_answers', TRUE),
  );

  // Fieldset texts.
  $form['field_quiz_fieldset_texts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Answer texts.'),
    '#collapsible' => TRUE,
  );

  // Correct answer text.
  $form['field_quiz_fieldset_texts']['field_quiz_text_answer_correct'] = array(
    '#type' => 'textfield',
    '#title' => t('The answer text if the answer was correct.'),
    '#description' => t('Enter the text that should be displayed if the answer was correct.'),
    '#default_value' => variable_get('field_quiz_text_answer_correct', t('Congratulations. This was the correct answer.')),
  );

  // Correct answer back link text.
  $form['field_quiz_fieldset_texts']['field_quiz_text_answer_correct_back_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('The back link text if the answer was correct.'),
    '#description' => t('Enter the text that should be displayed as back link text if the answer was correct.'),
    '#default_value' => variable_get('field_quiz_text_answer_correct_back_link_text', t('Back')),
  );

  // Wrong answer text.
  $form['field_quiz_fieldset_texts']['field_quiz_text_answer_wrong'] = array(
    '#type' => 'textfield',
    '#title' => t('The answer text if the answer was wrong.'),
    '#description' => t('Enter the text that should be displayed if the answer was wrong.'),
    '#default_value' => variable_get('field_quiz_text_answer_wrong', t('Sorry, wrong answer.')),
  );

  // Wrong answer back link text.
  $form['field_quiz_fieldset_texts']['field_quiz_text_answer_wrong_back_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('The back link text if the answer was wrong.'),
    '#description' => t('Enter the text that should be displayed as back link text if the answer was wrong.'),
    '#default_value' => variable_get('field_quiz_text_answer_wrong_back_link_text', t('Please retry.')),
  );

  return system_settings_form($form);
}