<?php

/**
 * Implements hook_drush_command().
 * Set up Tibetan sort field update drush command as "drush utsf"
 */
function tibarmy_sort_drush_command() {

  $commands['update-sort-fields'] = [
    'description' => 'A drush command to add/update all tib sort fields by saving all notice items',
    'aliases' => ['utsf'],
    'examples' => [
      'drush utsf' => 'Update all tib sort fields for notices',
    ],
  ];

  return $commands;
}

/**
 * Drush Update Tib Sort Fields
 * Populate the tib sort field for every notice node with a value that sorts by the standard methods
 *
 */
function drush_tibarmy_sort_update_sort_fields() {
  $node_type = 'notice';
  $result = db_query("SELECT nid FROM node WHERE type = :type", array(':type' => $node_type));
  $nids = $result->fetchCol();

  $totalct = count($nids);
  if ($totalct == 0) {
    drush_print("Warning: There are no notice nodes to update!");
    return;
  }

  drush_print("Adding Tibetan Sort field to $totalct items:");
  $done = 0;
  $chunks = array_chunk($nids, 100);
  foreach($chunks as $chunk) {
    foreach($chunk as $itemid) {
      $node = node_load($itemid);
      node_save($node);  // The Tib Sort value is added by the hook_presave() function in tibarmy_sort.module
    }
    $done += count($chunk);
    if ($done % 100 == 0) {
      drush_print("   Updated $done out of $totalct notices ...");
    }
  }
  drush_print("All notice nodes have been updated!");
}


