<?php 
function user_team($form, &$form_state){

		global $user;
		$path = drupal_get_path('module', 'user_profile');
		drupal_add_css($path . '/css/user-profile.css');	
		
		$form_state['team']['player'] = isset($form_state['team']['player']) ? $form_state['team']['player'] : 1;
		
		$form = array();
		$form['#action'] = url("team");
		$form['#method'] = 'POST';
		
		$form['wrapper'] = array(
			'#prefix' => '<div class="user-team-wrapper"><h3>Hello Captain '.$user->name.', Lets create your own team</h3>',
			'#suffix' => '</div>'
		);	

		$form['wrapper']['team-name'] = array(
			'#type' => 'textfield', 
			'#title' => t('Team Name'), 
			'#size' => 25, 
			'#maxlength' => 128, 
			'#description' => t('Please add team name'),	
		);

		$form['wrapper']['sports'] = array(			
			'#type' => 'select',
			'#title' => 'Sports',
			'#options' => array('0' => 'Please select the your sports'),			
			'#description' => t('Please select your sports passion'),			
		);		
		
		$form['wrapper']['team-picture'] = array(
			  '#type' 						=> 'managed_file',
			  '#title' 						=> t('Team Picture'),
			  '#description' 				=> t('Allowed extensions: gif png jpg jpeg'),
			  '#upload_location' 			=> 'public://team/',
			  '#upload_validators' 			=> array(
				'file_validate_extensions' 	=> array('gif png jpg jpeg'),
				// Pass the maximum file size in bytes
				'file_validate_size' => array(1 * 1024 * 1024),
			  ),			  
		);
		
		$form['wrapper']['fieldset'] = array(
			'#type' => 'fieldset',
			'#title' => t('Add players in your team'),
			'#collapsible' => TRUE, // Added
			'#collapsed' => FALSE,  // Added
		);
		
		$form['wrapper']['fieldset']['players'] = array(
			'#type' => 'container',
			'#tree' => TRUE,
			'#prefix' => '<div id="PLAYERS">',
			'#suffix' => '</div>',
		 );
		
		if ($form_state['team']['player']) {
			for ($i = 1; $i <= $form_state['team']['player']; $i++) {	

				$form['wrapper']['fieldset']['players'][$i]['email'] = array(
					'#type' => 'textfield', 
					'#title' => t('Email'), 
					'#size' => 30, 
					'#maxlength' => 200, 
					'#description' => t('Please add team member email'),	
				);	
				 
				$form['wrapper']['fieldset']['players'][$i]['first-name'] = array(
					'#type' => 'textfield', 
					'#title' => t('First Name'), 
					'#size' => 25, 
					'#maxlength' => 128, 
					'#description' => t('Please add team member first name'),	
				);

				$form['wrapper']['fieldset']['players'][$i]['last-name'] = array(
					'#type' => 'textfield', 
					'#title' => t('Last Name'), 
					'#size' => 25, 
					'#maxlength' => 128, 
					'#description' => t('Please add team member last name'),	
				);					

			}
			$form['wrapper']['fieldset']['player-add'] = array(
			'#type' => 'button',
			'#value' => t('Add Players'),
			'#ajax' => array(
				'callback' => 'ajax_user_team_player_submit',
				'wrapper' => 'PLAYERS',
				'method' => 'replace',
				'effect' => 'fade',
			),
		);
		}
		
			
		
		$form['wrapper']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),			
			'#ajax' => array(
				'callback' => 'ajax_user_team_submit',
				'wrapper' => 'MESSAGE',
				'method' => 'replace',
				'effect' => 'fade',
			),
			
		);	
		
		$form['wrapper']['container'] = array(
			'#prefix' => '<div id="MESSAGE">',
			'#suffix' => '</div>'
		);

		$form_state['team']['player']++;
		
		return $form;
		
}

function ajax_user_team_submit($form, &$form_state){	

	global $user;
	$team_name 			= $form_state['values']['team-name'];
	$sports				= $form_state['values']['sports'];
	
	//Create The node 
	$node = new stdClass();
	
	//Node type = team
	$node->type = 'team';
	
	//Node title
	$node->title = $team_name;

	//Node preparing
	node_object_prepare($node);	
	$node->language = LANGUAGE_NONE;
	
	//Node owner
	$node->uid = $user->uid;
	$node->status = 1; //(1 or 0): published or not
	$node->promote = 0; //(1 or 0): promoted to front page
	
	//node sportd
	$node->field_sports = $sports;	
	
	//Adding node in image
	$file = file_load($form_state['values']['team-picture']);
	if($file){
		$file_image = (object) array(
			'fid' => $file->fid,
			'uid' => $file->uid,
			'uri' => $file->uri,
			'filemime' => $file->filemime,
			'status' => $file->status,
		 );	
		$node->field_team_picture[LANGUAGE_NONE][0] = (array)$file_image;
	}
	
	if($form_state['team']['player'] > 0){
		for ($i = 0; $i < $form_state['team']['player']; $i++) {
			$email = $form_state['values']['players'][$i]['email'];
			if(!empty($email)){
				$node->field_players_email['und'][$i]['value'] = $form_state['values']['players'][$i++]['email'];				
			}
		}
	}
	
	//Save the node
	node_save($node);
	
	//---------------------------------------------------------------------------------------------
	
	
	//Add notification on home page 
	//Create The node 
	$node_notification = new stdClass();
	
	//Node type = team
	$node_notification->type = 'notification';
	
	//Node title
	$node_notification->title = $user->name." created the new team ".$team_name." !!!!";
	
	dpm($user);
	
	//Node userid
	$node_notification->field_user_id[und][0][value] = $user->uid;

	//Node preparing
	node_object_prepare($node_notification);	
	$node_notification->language = LANGUAGE_NONE;
	
	//Save the node
	node_save($node_notification);
	
	

}

function ajax_user_team_player_submit($form, &$form_state){

	$total_player		= $form_state['team']['player'];
	for ($i = 1; $i < $total_player; $i++) {	
	
		$player_first_name 	= $form_state['values']['players'][$i]['first-name'];
		$player_last_name 	= $form_state['values']['players'][$i]['last-name'];
		$player_email	 	= $form_state['values']['players'][$i]['email'];
		
		if(!empty($player_email)){
			$user = user_load_by_name($player_email);
			if(!$user){
				
				$new_user = array(
					'name' => $player_email,
					'pass' => uniqid(), // note: do not md5 the password
					'mail' => $player_email,
					'status' => 1,
					'init' => $player_email,
					'field_first_name' => array(LANGUAGE_NONE => array(array('value' => $player_first_name))),
					'field_last_name'  => array(LANGUAGE_NONE => array(array('value' => $player_last_name)))
				);
			
				user_save(null, $new_user);
				$html = "New player created !!!";		
			} 
			else {
				$html = "Player already exist !!!";
			}
		}
	}
	
	return $form['wrapper']['fieldset']['players'];
	
}


