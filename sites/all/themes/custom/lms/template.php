<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 * 
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */
 
 $profile = "profile";
 
 function lms_theme() {
  $items = array();  
  $items['user_register_form'] = array(
    'render element' => 'form',
    'path' => drupal_get_path('theme', 'lms') . '/templates',
    'template' => 'user-register-form',    
  );
  
  $items['profile_node_form'] = array(
	'path' => drupal_get_path('theme', 'lms') . '/templates',
    'template' => 'profile-node-form',    
  );
  
  return $items;
}

function lms_block_view_alter(&$data, $block) {
	
	if(user_is_anonymous()) {  
		if ($block->module == 'block' && $block->delta == 3) {			
		  $form = drupal_get_form('user_register_form');
		  $data['content'] 	= drupal_render($form);	  
		}			
	}else{
		if ($block->module == 'block' && $block->delta == 3) {
			$data['content'] 	= "";	  
		}
	}

}

/*function lms_preprocess_page(&$var) {
	if (isset($var['node']) && $var['node']->type == $profile) {
		 $var['theme_hook_suggestions'][] = "profile_node_form";	
	}
}*/