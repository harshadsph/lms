<?php 

function user_home(){
	return "";
}

function user_profile($form){

	global $user;
	$path = drupal_get_path('module', 'user_profile');
	drupal_add_css($path . '/css/user-profile.css');	

		$form = array();
		$form['#action'] = url("user-profile");
		$form['#method'] = 'POST';
		
		$form['wrapper'] = array(
			'#prefix' => '<div class="user-profile-wrapper"><h3>Hi '.$user->name.', Lets complete your sports profile</h3>',
			'#suffix' => '</div>'
		);		

		$form['wrapper']['sports'] = array(			
			'#type' => 'select',
			'#title' => 'Sports Passion',
			'#options' => array('0' => 'Please select the your sports'),			
			'#description' => t('Please select your sports passion'),			
		);		
		
		$form['wrapper']['country'] = array(			
			'#type' => 'select',
			'#title' => 'Country',
			'#options' => array('0' => 'Please select the country'),			
			'#description' => t('Please select your country'),
		);
		
		$form['wrapper']['postal-code'] = array(
			'#type' => 'textfield',
			'#title' => t('Postal Code'),
			'#size' => 30,
			'#description' => t('Please enter your postal code'),			
		);
		 

		$form['wrapper']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),			
			'#ajax' => array(
				'callback' => 'ajax_user_profile_submit',
				'wrapper' => 'MESSAGE',
				'method' => 'replace',
				'effect' => 'fade',
			),
			
		);	
		
		$form['wrapper']['container'] = array(
			'#prefix' => '<div id="MESSAGE">',
			'#suffix' => '</div>'
		); 
		
		return $form;
}

function ajax_user_profile_submit(){
	$sports 		= $_POST['sports'];
	$country 		= $_POST['country'];
	$postal_code	= $_POST['postal-code'];
	$uid			= $GLOBALS['user']->uid;
	
	//Change the new user flag to 1 as user already added is profile in system.
	$user_profile_master_updated = 	db_update('user_profile_master')
					->fields(array('new_user' => 1,))
					->condition ('uid', $uid, '=')
					->execute();
					
	//Add the profile information in profile table 			
	$upsert = db_merge('user_profile_details')
					->key(array('uid' => $uid))
					->fields(array(
							'uid' 			=> $uid,
							'sports' 		=> $sports,
							'country'		=> $country,
							'postal_code'	=> $postal_code,
						))
					->execute();
					
	$html = "<div class='user-profile-message'>Great job !!! Your profile has been submitted !!!! <a href='http://lms.com.sg/' style='float:right'><button>Next</button></a></div>";
	return $html;
}
