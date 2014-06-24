<?php

/**
 * @file
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Special Topic Nodes List'),
  'callback' => 'special_topic_nodes_list_render'
);


function special_topic_nodes_list_render($nodes, $extra) {
  $links = array();
  $readmore = '';
  if (!empty($extra) && valid_url($extra, TRUE)) {
    // add readmore link
    $readmore = '<div class="link-wrapper"><a target="_blank" href="' . $extra . '">' . t('Read more') . '</a></div>';
  }
  foreach ($nodes as $node) {
    if (empty($node)) {
      continue;
    }
    $links[] = array(
      'title' => $node->title,
      'href' => 'node/' . $node->nid,
    );
  }
  return theme('links', array('links' => $links)) . $readmore;
}
