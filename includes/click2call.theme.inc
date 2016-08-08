<?php

/*
 * Retrieve and theme phone numbers selection
 */
function theme_click2call_phone_numbers($vars) {
  $field_name = $vars['field_name'];
  $delta = $vars['delta'];

  global $user;
  $op = variable_get('click2call_number', array('type'));
  if ($vars['content_type']) {
    if ($field_settings = content_fields($field_name, $vars['content_type'])) {
      $op = $field_settings['click2call_number'];
    }
  }
  foreach ($op as $key => $call_option) {
    if (0 === $call_option) {
      unset($op[$key]);
    }
  }
  $num = count($op);
  if ($num == 1 || $user->uid == 0) {
    //If only one choice is available or user is anonymous then make the option preselected
    $checked = 'checked="checked"';
  }
//  $output = '';
//  $output .= '<div class="click2call-' . $field_name . '-' . $delta . '-phone">' . $option . '</div>';
  return '<div class="click2call-' . $field_name . '-' . $delta . '-phone"></div>';
  return $output;
}
