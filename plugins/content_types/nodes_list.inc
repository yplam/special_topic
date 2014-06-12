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
	'field_name' => 'field_special_topic',
    'special_position' => '',
	'theme_name' => 'special_topic_list'
  ),
);

/**
 * Render the custom content type.
 */
function special_topic_nodes_list_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $node = $context->data;
  if(!field_info_instance('node', $conf['field_name'], $conf['node_type'])){
	return FALSE;
  }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $conf['node_type'])
    ->fieldCondition($conf['field_name'], 'special_id', $node->nid, '=')
	->propertyCondition('status', 1);
  if($conf['special_position']){
    $query->fieldCondition($conf['field_name'], 'special_position', $conf['special_position'], '=');
  }
        
  $result = $query->execute();
  
  if(empty($result['node'])){
    return FALSE;
  }
  $nodes = array();
  foreach($result['node'] as $val){
	$nodes[] = node_load($val->nid);
  }

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'special_topic';
  $block->title   = 'Nodes List';
  $block->content = theme($conf['theme_name'], $nodes);
  $block->delta   = $node->nid;

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function special_topic_nodes_list_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['node_type'] = array(
    '#title' => t('The node type'),
    '#type' => 'textfield',
    '#default_value' => $conf['node_type'],
	'#required' => TRUE,
  );
  $form['field_name'] = array(
    '#title' => t('The name of node field'),
    '#type' => 'textfield',
    '#default_value' => $conf['field_name'],
	'#required' => TRUE,
  );
  $form['theme_name'] = array(
    '#title' => t('Theme name to render the nodes'),
    '#type' => 'textfield',
    '#default_value' => $conf['theme_name'],
	'#required' => TRUE,
  );
  $form['special_position'] = array(
    '#title' => t('The name of this position to filter nodes'),
	'#description' => t('If left empty, this will load all nodes of this special topic'),
    '#type' => 'textfield',
    '#default_value' => $conf['special_position'],
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
