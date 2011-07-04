.. 2011/07/04 jptomo 8ac37d1c76f2d6ad73fd1d24b73ee159542c719d

.. index::
   single: Configuration Reference; Swiftmailer

SwiftmailerBundle İ’è
===============================

‚·‚×‚Ä‚Ìİ’è’l‚Ìà–¾‚¨‚æ‚Ñ‰Šú’l
--------------------------

.. configuration-block::

    .. code-block:: yaml

        swiftmailer:
            transport:            smtp
            username:             ~
            password:             ~
            host:                 localhost
            port:                 false
            encryption:           ~
            auth_mode:            ~
            spool:
                type:                 file
                path:                 %kernel.cache_dir%/swiftmailer/spool
            sender_address:       ~
            antiflood:
                threshold:            99
                sleep:                0
            delivery_address:     ~
            disable_delivery:     ~
            logging:              true