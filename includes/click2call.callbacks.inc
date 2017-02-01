<?php

function click2call_call($c2c_form_build_id) {
  $form_build_info = click2call_get_form_build_info($c2c_form_build_id);

  if ('click2call_call_form' == $form_build_info['form_id'] && isset($form_build_info['args'])) {
    $field_name = $form_build_info['args']['module'];
    $module = $form_build_info['args']['module'];
    $delta = $form_build_info['args']['delta'];
    $c2c_key = $form_build_info['args']['c2c_key'];

    $name = variable_get('click2call_block_script_desc_' . $delta, t('Click here to call'));
    $caller_name = t('Click2Call Block ' . $delta);
    $caller_uid = 1;

    global $user;
    // TODO: Add back GET phone
//    $number = $_GET['phone'];
    $params = $form_build_info['args']['params'] + array(
      'module' => $module,
      'delta' => $delta,
      'uid' => $user->uid,
      'name' => $name,
//      'number' => $number,
      'dest_name' => $user->name,
      'caller_uid' => $caller_uid,
//      'script_name' => $script_name,
      'click2call_key' => $c2c_key,
  //    'click2call_phoneto' => $phone_to,
      'caller_name' => $caller_name,
//      'click2call_call_form_params' => $form_build_info['args']['params'],
    );
    // Allow any modules to alter the params passed in, even script name

    // TODO: Click2Call Webform will alter these params to put first name in
    drupal_alter('click2call_params', $params);

    $script = VoipScript::loadScript($params['script_name'], $params);

    $call = new VoipCall();
    $call->setDestNumber($params['number_1']);
    $call->setDestName($params['dest_name']);
    // Set caller number if it exists.
    $call->setCallerName($params['caller_name']);
    if (array_key_exists('caller_number', $params)) {
      $call->setCallerNumber($params['caller_number']);
    }
    $call->setScript($script);
    $call->save();

    // Dial the call.
    voip_dial($call);

    // Log call with watchdog
    $type = 'click2call';
    $message = t(
      'New Click2Call message recorded by !username to phone !number. See voip call cid !call_cid.',
      array(
        '!username' => $user->name,
        '!number' => $params['number'],
        '!call_cid' => $call->getCid(),
      )
    );
    watchdog($type, $message);

    // Invoke a hook to allow child modules to do something with the call
    module_invoke_all('click2call_call', $call, $c2c_key);

    return array('callCid' => $call->getCid());
  }
}

/**
 * AJAX Callback function
 * Hangups the call and returns status (true/false).
 */
function click2call_hangup($call_cid) {
  $call = VoipCall::load($call_cid);
  $status = voip_hangup($call);
  module_invoke_all('click2call_hangup', $call);
  return array('status' => $status);
}

/**
 * AJAX Callback function
 * Returns status(true/false) and url for recording based on call_id.
 */
function click2call_status($call_cid) {
  global $user;
	$call = VoipCall::load($call_cid);
	//Check for status only if call is finished
	if ($call->isHangup()==TRUE) {
    if ($call->getCallStatus() != VoipCall::COMPLETED) {
      if ($call->getCallStatus() == VoipCall::ERROR) {
        $failed_message='<div class="error">' . $call->getErrorMessage() . '</div>';
      }
      elseif ($call->getCallStatus()=='to_hangup' || $call->getCallStatus() == VoipCall::CANCELED) {
        //We display message for all statuses except hangup
        $failed_message='<div class="messages status">' . $call->getCallStatusDescription() . '</div>';
      }
      else {
        $failed_message='<div class="error">' . $call->getCallStatusDescription() . '</div>';
      }
      return array('status' => 'failed', 'message' => $failed_message);
    }
    $message='<div class="messages status">' . $call->getCallStatusDescription() . '</div>';
    // TODO: VoipCall is Completed, invoke a HOOKS
    module_invoke_all('click2call_completed', $call);
    return array('status' => 'success' , 'message' => $message);
  }
  else {
   return array('status' => 'calling');
  }
}
