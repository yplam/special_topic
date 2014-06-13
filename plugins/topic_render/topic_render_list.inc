<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Special Topic Nodes List'),
  'callback' => 'special_topic_nodes_list_render'
);


function special_topic_nodes_list_render($nodes, $extra){
  $links = array();
  foreach($nodes as $node){
    if(empty($node)){
	  continue;
	}
    $links[] = array(
	  'title' => $node->title,
	  'href' => 'node/'.$node->nid,
	);
  }
  return theme('links', array('links'=>$links));
}