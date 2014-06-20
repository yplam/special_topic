<?php

/**
 * Render Links on a Special Topic Page
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Links List'),
  'description' => t('List of external links.'),
  'category' => t('Special Topic'),
  'defaults' => array(
    'theme_name' => '',
    'links' => '',
    'extra' => '',
    'newpage' => 0,
  ),
);

/**
 * Select node list and render with a theme.
 */
function special_topic_links_list_content_type_render($subtype, $conf, $panel_args, $context) {
  if(empty($conf['links'])){
    return FALSE;
  }
  $theme = 'links';
  $links = array();

  $links_str = explode("\n", $conf['links']);
  $attributes = array();
  if(!empty($conf['newpage'])){
    $attributes['target'] = '_blank';
  }
  foreach($links_str as $val){
    $split_pos = strrpos($val, "|");
    if($split_pos === FALSE){
      continue;
    }
    $links[] = array(
      'title' => trim(substr($val, 0, $split_pos)),
      'href' => trim(substr($val, $split_pos+1)),
      'attributes' => $attributes,
    );
  }

  $render_types = special_topic_get_render_types();
  $content = '';
  if( !empty($render_types[$conf['theme_name']]) && function_exists($render_types[$conf['theme_name']]['callback'])){
    $function_name = $render_types[$conf['theme_name']]['callback'];
    $content = $function_name($links, $conf['extra']);
  }
  else{
    $content = theme('links', array('links'=>$links));
  }
  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'special_topic';
  $block->title   = 'Nodes List';
  $block->content = $content;

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function special_topic_links_list_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $render_types = special_topic_get_render_types();
  $render_types_option = array();
  $render_types_option[0] = 'Links';
  foreach($render_types as $key=>$val){
    $render_types_option[$key] = $val['title'];
  }
  $form['theme_name'] = array(
    '#title' => t('Theme name to render the nodes'),
    '#type' => 'select',
    '#default_value' => $conf['theme_name'],
    '#options' => $render_types_option,
    '#required' => TRUE,
  );
  $form['links'] = array(
    '#title' => t('Links to display.'),
    '#type' => 'textarea',
    '#description' => 'Format: text|url. One link each line.',
    '#default_value' => $conf['links'],
    '#required' => TRUE,
  );
  $form['newpage'] = array(
    '#title' => t('Open link in new page'),
    '#type' => 'checkbox',
    '#default_value' => $conf['newpage'],
  );
  $form['extra'] = array(
    '#title' => t('Extra string pass to the theme function'),
    '#type' => 'textarea',
    '#default_value' => $conf['extra'],
  );

  return $form;
}

/**
 * form validate.
 */
function special_topic_links_list_content_type_edit_form_validate($form, &$form_state) {
  $links = explode("\n", $form_state['values']['links']);
  $curline = 1;
  foreach($links as $val){
    $split_pos = strrpos($val, "|");
    if($split_pos === FALSE){
      form_set_error('links', 'No | on line '.$curline);
    }
    $url = substr($val, $split_pos+1);
    if(!valid_url($url, TRUE)){
      form_set_error('links', 'Wrong url format on line '.$curline);
    }
    $curline ++;
  }
}

/**
 * Submit handler for the custom type settings form.
 */
function special_topic_links_list_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function special_topic_links_list_content_type_admin_title($subtype, $conf, $context) {
  return t('Special topic external links');
}
