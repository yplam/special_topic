<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Special Topic Nodes List'),
  'icon' => 'icon_node.png',
  'description' => t('The list of the choose nodes.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  
  'category' => t('Special Topic'),
  'defaults' => array(
    'node_type' => '',
    'is_promoted' => 0,
	'count' => 0,
	'field_name' => 'field_special_topic',
    'special_position' => '',
	'theme_name' => 'special_topic_list',
	'extra' => ''
  ),
);

/**
 * Select node list and render with a theme.
 */
function special_topic_nodes_list_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $node = $context->data;
  // test if this node type and field_name
  if(!field_info_instance('node', $conf['field_name'], $conf['node_type'])){
	return FALSE;
  }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $conf['node_type'])
    ->fieldCondition($conf['field_name'], 'special_id', $node->nid, '=')
	->propertyCondition('status', 1);
  $conf['is_promoted'] && $query->propertyCondition('promoted', 1);
  $conf['special_position'] && $query->fieldCondition($conf['field_name'], 'special_position', $conf['special_position'], '=');
  $conf['is_promoted'] && $query->propertyCondition('promoted', 1);
  $conf['count'] && $query->range(0, (int)$conf['count']);
        
  $result = $query->execute();
  
  if(empty($result['node'])){
    return FALSE;
  }
  $nodes = array();
  foreach($result['node'] as $val){
	$nodes[] = node_load($val->nid);
  }
  $render_types = special_topic_get_render_types();
  $content = '';
  if( !empty($render_types[$conf['theme_name']]) && function_exists($render_types[$conf['theme_name']]['callback'])){
    $function_name = $render_types[$conf['theme_name']]['callback'];
    $content = $function_name($nodes, $conf['extra']);
  }
  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'special_topic';
  $block->title   = 'Nodes List';
  $block->content = $content;
  $block->delta   = $node->nid;

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function special_topic_nodes_list_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  // get all node type
  $node_types = node_type_get_types();
  $node_types_option = array();
  foreach($node_types as $val){
    $node_types_option[$val->type] = $val->name;
  }  
  $form['node_type'] = array(
    '#title' => t('The node type'),
    '#type' => 'select',
    '#default_value' => $conf['node_type'],
	'#options' => $node_types_option,
	'#required' => TRUE,
  );
  $form['is_promoted'] = array(
    '#title' => t('Only select promoted nodes'),
    '#type' => 'checkbox',
    '#default_value' => $conf['is_promoted'],
  );
  $form['count'] = array(
    '#title' => t('Limit node count, 0 for no limit'),
    '#type' => 'textfield',
    '#default_value' => $conf['count'],
	'#required' => TRUE,
  );
  
  // field option
  $field_types = field_info_field_map();
  $field_types_option = array();
  foreach($field_types as $key => $val){
    if($val['type'] == 'field_special_topic'){
	  $field_types_option[$key] = $key . ' : ' . implode(',', $val['bundles']['node']); 
	}
  }
  $form['field_name'] = array(
    '#title' => t('The name of node field'),
    '#type' => 'select',
    '#default_value' => $conf['field_name'],
	'#options' => $field_types_option,
	'#required' => TRUE,
  );
  
  $render_types = special_topic_get_render_types();
  $render_types_option = array();
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
  $form['special_position'] = array(
    '#title' => t('The name of this position to filter nodes'),
	'#description' => t('If left empty, this will load all nodes of this special topic'),
    '#type' => 'textfield',
    '#default_value' => $conf['special_position'],
  );
  $form['extra'] = array(
    '#title' => t('Extra string pass to the theme function'),
    '#type' => 'textarea',
    '#default_value' => $conf['extra'],
  );

  return $form;
}

/**
 * Submit handler for the custom type settings form.
 */
function special_topic_nodes_list_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function special_topic_nodes_list_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" topic nodes list', array('@s' => $context->identifier));
}
