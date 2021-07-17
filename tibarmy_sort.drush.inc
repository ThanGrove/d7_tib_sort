<?php

/**
 * Implements hook_drush_command().
 * Set up the kmaps field reindexing drush command as "drush ikf"
 */
function tibarmy_sort_drush_command() {

  $commands['update-tib-sort-fields'] = [
    'description' => 'A drush command to update all tib sort fields by saving all notice items',
    'aliases' => ['utsf'],
    'examples' => [
      'drush utsf' => 'Update all tib sort fields',
    ],
  ];

  $commands['index-collection'] = [
    'description' => "A drush command to index all items in a collection",
    'aliases' => ['kmfic'],
    'argument' => [
      'collid' => 'The node ID for the collection to index',
    ],
    'examples' => [
      'drush kmfic 3' => 'Index collection with node ID of 3 on this site',
    ],
  ];
  $site_type = (function_exists('shanti_general_get_site_type')) ?  shanti_general_get_site_type() : 'unknown';
  $commands['delete-all-records'] = [
    'description' => "A drush command to delete all SOLR records for this site, $site_type",
    'aliases' => ['kmfd'],
    'examples' => [
      'drush kmfd' => 'Returns the current sites name',
    ],
  ];
  return $commands;
}

/**
 * Drush Index Kmaps Fields command
 * Call the batch fucntion for indexing all kmaps with a value of TRUE for $is_drush
 *
 * @param int $start_ind
 */
function drush_update_tib_sort_fields() {
  $node_type = 'notice';
  $result = db_query("SELECT nid FROM node WHERE type = :type", array(':type' => $node_type));
  $nids = $result->fetchCol();
  // Need to fix up this

  $totalct = count($nids);
  if ($totalct == 0) {
    drush_print("Warning: There are no items in this collection. Nothing indexed!");
    return;
  }

  drush_print("Adding Tibetan Sort field to $totalct items:");
  $done = 0;
  $chunks = array_chunk($nids, 100);
  foreach($chunks as $chunk) {
    foreach($chunk as $itemid) {
      _shanti_kmaps_fields_push_solr_doc($itemid);
    }
    $done += count($chunk);
    if ($done % 100 == 0) {
      drush_print("   Indexed $done out of $totalct nodes...");
    }
  }
  drush_print("All nodes in collection $cid have been reindexed!");
}


