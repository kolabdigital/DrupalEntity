<?php

/**
 * @file
 * Default theme implementation to display a lead details.
 * @ingroup themeable
 */
?>

<?php $details = reset($variables); ?>

<div class="lead-details">

  <div class="name">
    <?php if($details['title']) print ucfirst($details['title']) . ' '; ?>
    <?php print $details['first_name'] . ' ' . $details['surname']; ?>
  </div>

  <div class="address">
    <?php
      $address = array($details['address']);
      if($details['address2']) $address[] = $details['address2'];
      if($details['county']) $address[] = $details['county'];
      print '<span>' . implode('</span><span class="sep">,</span><span>', $address) . '</span>';
    ?>
    <?php if($details['postcode']): ?>
      <span class="postcode"><?php print $details['postcode']; ?></span>
    <?php endif; ?>
  </div>

  <?php if($details['mail'] || $details['phone_mobile'] || $details['phone']): ?>
    <div class="contact">
      <?php if($details['mail']): ?>
        <span class="mail"><?php print l($details['mail'], 'mailto:' . $details['mail']); ?></span>
      <?php endif; ?>
      <?php if($details['phone_mobile'] || $details['phone']): ?>
        <div class="phones">
        <?php 
          $phones = array();
          if($details['phone_mobile']) $phones[] = $details['phone_mobile'];
          if($details['phone']) $phones[] = $details['phone'];
          print '<span>' . implode('</span><span class="sep">,</span><span>', $phones) . '</span>';
        ?>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>
</div>
