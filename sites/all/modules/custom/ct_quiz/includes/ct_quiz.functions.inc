<?php
/*
* Function to obtain current published nodes
*/
function published_quiz_nodes(){
  $query = new EntityFieldQuery();

  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'quiz')
    ->propertyCondition('status', 1, '=')
    ->execute();

  if ($result) {
    return $result['node'];
  }else{
    return null;
  }
}