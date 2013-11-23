.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

.. index::
   pair: Monolog; Configuration Reference

MonologBundle 設定
==================

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:

                # 設定例:
                syslog:
                    type:                stream
                    path:                /var/log/symfony.log
                    level:               ERROR
                    bubble:              false
                    formatter:           my_formatter
                    processors:
                        - some_callable
                main:
                    type:                fingers_crossed
                    action_level:        WARNING
                    buffer_size:         30
                    handler:             custom
                custom:
                    type:                service
                    id:                  my_handler

                # 初期のオプションと値。"my_custom_handler"を例に。
                my_custom_handler:
                    type:                 ~ # 必須項目
                    id:                   ~
                    priority:             0
                    level:                DEBUG
                    bubble:               true
                    path:                 "%kernel.logs_dir%/%kernel.environment%.log"
                    ident:                false
                    facility:             user
                    max_files:            0
                    action_level:         WARNING
                    activation_strategy:  ~
                    stop_buffering:       true
                    buffer_size:          0
                    handler:              ~
                    members:              []
                    channels:
                        type:     ~
                        elements: ~
                    from_email:           ~
                    to_email:             ~
                    subject:              ~
                    mailer:               ~
                    email_prototype:
                        id:                   ~ # 必須項目 (email_prototypeの場合)
                        method:               ~
                    formatter:            ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="syslog"
                    type="stream"
                    path="/var/log/symfony.log"
                    level="error"
                    bubble="false"
                    formatter="my_formatter"
                />
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action-level="warning"
                    handler="custom"
                />
                <monolog:handler
                    name="custom"
                    type="service"
                    id="my_handler"
                />
            </monolog:config>
        </container>

.. note::

    プロファイラが有効な場合、プロファイラにログメッセージを保存するようハンドラに追加されます。プロファイラは "debug" を既に名前で利用しているため、この名前を設定で利用することはできません。

.. 2013/11/23 yositani2002 d11327b2c28ebb71b9cdc1b5cf5879183905b3ad