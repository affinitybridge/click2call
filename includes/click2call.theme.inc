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
   $option = '';
   $output = '';
   foreach ($op as $call_option) {
     switch ($call_option) {
       case 'type':
//         $option .= '<input type="radio" name="click2call-' . $field_name . '-' . $delta . '-phone" value="type" ' . $checked . '/><input type="text" id="click2call-' . $field_name . '-' . $delta . '-type-phone" class="click2call-type"/><br/>';
         break;

     }
   }
   $output .= '<div class="click2call-' . $field_name . '-' . $delta . '-phone">' . $option . '</div>';
   return $output;
 }


/*
 * Retrieve and theme phone numbers selection
 */
 /*
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
  $option = '';
  $output = '';
  foreach ($op as $call_option) {
    switch ($call_option) {
      case 'default':
        if (module_exists('voipnumber')) {
          $phone = VoipNumber::getDefaultNumberFromUid();
          if (empty($phone)) {
            if ($num == 2) {
              //If only 2 choices are available then make the other selected
              $checked = 'checked="checked"';
            }
          }
          else {
            $option .= '<input type="radio" name="click2call-' . $field_name . '-' . $delta . '-phone" value="' . $phone->getNumber() . '" checked="checked"/>Default (' . $phone->getNumber() . ')<br/>';
          }
        }
        break;

      case 'type':
        $option .= '<input type="radio" name="click2call-' . $field_name . '-' . $delta . '-phone" value="type" ' . $checked . '/><input type="text" id="click2call-' . $field_name . '-' . $delta . '-type-phone" class="click2call-type"/><br/>';
        break;

      case 'voipnumber':
        if (module_exists('voipnumber')) {
          $numbers = VoipNumber::getNumbersFromUid();
          //Numbers could be empty, check.
          if (empty($numbers)) {
            continue;
          }
          $option .= '<input type="radio" name="click2call-' . $field_name . '-' . $delta . '-phone" value="voipnumber" ' . $checked . '/>';
          $option .= '<select class="click2call-voipnumber" id="click2call-' . $field_name . '-' . $delta . '-select">';
          foreach ($numbers as $number) {
            $option .= "<option value='".$number->getNumber()."'> ".$number->getName()." </option>";
          }
          $option .= '</select><br/>';
        }
        break;
    }
  }
  $output .= '<div class="click2call-' . $field_name . '-' . $delta . '-phone">' . $option . '</div>';
  return $output;
}
*/
