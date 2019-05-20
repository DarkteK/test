<?php
/**
 * @file
 * Miscellaneous utility functions to support WWE site.
 */

/**
 * Wipes all content and all terms in Author, Venue, Tags.
 */
function wwe_master_wipe() {
  // Don't let this run for more than a day, currently takes only 15-30 minutes.
  set_time_limit(86400);

  // Define the vocabularies IDs to be wiped, used in WHERE clauses below.
  $vids = "(1, 2, 6, 9, 16)";

  // Special handling for path scrubbing.
  //
  // Get a list of all taxonomy term paths.
  $paths = array();
  $terms = db_query("SELECT tid FROM `taxonomy_term_data` WHERE `vid` IN $vids");
  while ($tid = $terms->fetchColumn()) {
    $paths[] = "taxonomy/term/$tid";
  }

  // Define the LIKE pattern for scrubbing entity paths.
  $like = "RLIKE '^/?(node|comment|file)/.+'";

  // Two tables from which to scrub paths.
  $tables = array(
    'redirect'  => 'redirect',
    'url_alias' => 'alias',
  );
  foreach ($tables as $table => $column) {
    $where = "WHERE `source` $like
      OR `$column` $like
      OR `source` IN (:paths)
      OR `$column` IN (:paths)";
    $params = array(':paths' => $paths);
    _wwe_master_query($table, $where, $params);
  }

  // Tables from which we want to delete rows, grouped by WHERE clause.
  $wipe = array(

    // No conditions.
    "WHERE 1=1" => array(
      'cache%',
      'comment',
      'content_lock',
      'ctools_css_cache',
      'ctools_object_cache',
      'fe_paths_usage',
      'file_managed',
      'file_metadata',
      'file_usage',
      'history',
      'imagefield_focus_file',
      'node',
      'node_comment_statistics',
      'node_revision',
      'queue',
      'scheduler',
      'search_node_links',
      'search%',
      'tax_tmp',
      'wwe_update_log',
    ),

    // Filter by entity type.
    "WHERE `entity_type` IN ('node', 'comment', 'taxonomy_term', 'file')" => array(
      'entity_embed',
      'entity_modified',
      'entity_translation',
      'entity_translation_revision',
      'field_data_%',
      'field_deleted_data_%',
      'field_revision_%',
      'flag',
      'flag_counts',
      'flagging',
      'metatag',
      'pathauto_i18n',
      'pathauto_state',
    ),

    // Node Access grants; nid 0 has special meaning here.
    "WHERE `nid` > 0" => array(
      'node_access',
    ),

    // Taxonomy metadata.
    "INNER JOIN `taxonomy_term_data` ON `wipe`.`tid` = `taxonomy_term_data`.`tid` WHERE `vid` IN $vids" => array(
      'taxonomy_index',
      'taxonomy_term_hierarchy',
    ),

    // Taxonomy data.
    "WHERE `vid` IN $vids" => array(
      'taxonomy_term_data',
    ),
  );

  foreach ($wipe as $clauses => $tables) {
    foreach ($tables as $pattern) {
      _wwe_master_query($pattern, $clauses);
    }
  }
}

/**
 * Executes a SQL query, outputs message, and resets auto_increment.
 *
 * @param string $pattern
 *   Table name or LIKE pattern to match multiple tables.
 * @param string $clauses
 *   SANITIZED query clauses. E.g., a WHERE clause.
 * @param array $params
 *   Query paramters. See db_query().
 */
function _wwe_master_query($pattern, $clauses, $params = array()) {
  global $databases;

  // Define a function to use for user output.
  $print = function_exists('drush_print') ? 'drush_print' : 'drupal_set_message';

  // Get the name of our database, for use in table listing below.
  $database = $databases['default']['default']['database'];

  // Handle wildcards.
  $tables = db_query("SHOW TABLES WHERE `Tables_in_$database` LIKE :pattern", array(
    ':pattern' => $pattern,
  ))->fetchCol();

  if (empty($tables)) {
    $print("WARNING: Table pattern \"$pattern\" did not match any tables.");
    return;
  }

  foreach ($tables as $table) {
    $print("$table: Deleting rows...");

    // Yes, this query is unsanitized here, but the WHERE statement is sanitized
    // everywhere this function is called.
    $query = db_query("DELETE `wipe` FROM `$table` `wipe` $clauses", $params);
    $print("$table: Deleted " . $query->rowCount() . ' rows.');

    // Is there an auto-increment field on this table?
    $column = db_query("
      SELECT COLUMN_NAME
      FROM INFORMATION_SCHEMA.COLUMNS
      WHERE TABLE_NAME = :table
        AND DATA_TYPE = 'int'
        AND COLUMN_DEFAULT IS NULL
        AND IS_NULLABLE = 'NO'
        AND EXTRA like '%auto_increment%'",
      array(':table' => $table))->fetchField();

    // Reset auto increment to the lowest possible.
    if (!empty($column)) {
      $max = db_query("SELECT MAX(`$column`) FROM `$table`")->fetchField();
      if (is_null($max)) {
        $max = 0;
      }
      db_query("ALTER TABLE `$table` AUTO_INCREMENT = $max");
      $print("$table: Reset AUTO_INCREMENT column $column to $max.");
    }
  }
}
