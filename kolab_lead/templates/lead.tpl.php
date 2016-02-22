<?php

/**
 * @file
 * Default theme implementation to display a lead.
 * @ingroup themeable
 */
?>
<article id="lead-<?php print $lead->lid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php
    // $timescale = field_get_items('lead', $lead, 'field_timescale_for_moving');
    // debugVar($timescale);
  ?>

  <div class="content"<?php print $content_attributes; ?>>

    <?php if($page): ?>

      <?php
        print render($content);
        print "<br />";
      ?>

      <?php print t('This is full node template example'); ?>

    <?php else: ?>

      <h2<?php print $title_attributes; ?>>
        <?php if ($url): ?>
          <a href="<?php print $url; ?>"><?php print $title; ?></a>
        <?php else: ?>
          <?php print $title; ?>
        <?php endif; ?>
      </h2>

    <?php endif; ?>

  </div>  

</article>