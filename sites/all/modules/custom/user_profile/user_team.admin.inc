<?php 
function user_team($form){

		global $user;
		$path = drupal_get_path('module', 'user_profile');
		drupal_add_css($path . '/css/user-profile.css');	
		
		$form = array();
		$form['#action'] = url("user-team");
		$form['#method'] = 'POST';
		
		$form['wrapper'] = array(
			'#prefix' => '<div class="user-team-wrapper"><h3>Hello Captain '.$user->name.', Lets create your own team</h3>',
			'#suffix' => '</div>'
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
