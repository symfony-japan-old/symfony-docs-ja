.. 2013/10/08 monmonmon 4346f75f05a5ee010d0148ea251e99c7f6a02c38

.. index::
   single: Logging

.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/10/08


ログメッセージを複数ファイルへ出力させるには
============================================

Symfony Standard Edition のロギングはいくつかのチャネルからなります。
``doctrine``, ``event``, ``security``, ``request`` などです。
チャネルは、異なるタイプのログメッセージを管理するために用意されたものです。
これらのチャネルは、コンテナから取得する logger サービス (``monolog.logger.XXX``) に関連付けたり、特定のサービスに関連付けたりすることが出来ます。

デフォルトで、Symfony2 は全てのログメッセージを（チャネルに関係なく）１つのファイルに書き込みます。


チャネルを別のハンドラへ切り替える
----------------------------------

では仮に、``doctrine`` チャネルのログを別ファイルに書き込みたいとします。

コンフィギュレーションファイルに以下のように新しい doctrine ハンドラを追加します。
main ハンドラは ``doctrine`` チャネル以外を、doctrine ハンドラは ``doctrine`` チャネルのみを出力するように設定します。

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:
                main:
                    type: stream
                    path: /var/log/symfony.log
                    channels: !doctrine
                doctrine:
                    type: stream
                    path: /var/log/doctrine.log
                    channels: doctrine

    .. code-block:: xml

        <monolog:config>
            <monolog:handlers>
                <monolog:handler name="main" type="stream" path="/var/log/symfony.log">
                    <monolog:channels>
                        <type>exclusive</type>
                        <channel>doctrine</channel>
                    </monolog:channels>
                </monolog:handler>

                <monolog:handler name="doctrine" type="stream" path="/var/log/doctrine.log" />
                    <monolog:channels>
                        <type>inclusive</type>
                        <channel>doctrine</channel>
                    </monolog:channels>
                </monolog:handler>
            </monolog:handlers>
        </monolog:config>

YAML による設定の書き方
-----------------------

YAML による設定の書き方の例です。様々な方法で書くことが出来ます。

.. code-block:: yaml

    channels: ~    # 全てのチャネルを含む

    channels: foo  # "foo" チャネルのみ含む
    channels: !foo # "foo" 以外のすべてのチャネルを含む

    channels: [foo, bar]   # "foo", "bar" チャネルを含む
    channels: [!foo, !bar] # "foo", "bar" 以外の全てのチャネルを含む

    channels:
        type:     inclusive     # elements にリストしたチャネルのみ含む
        elements: [ foo, bar ]
    channels:
        type:     exclusive     # elements にリストしたチャネル以外のすべてのチャネルを含む
        elements: [ foo, bar ]

サービスのチャネルを設定する
----------------------------

ログの出力先チャネルはサービス毎に選択可能です。
サービスの設定に ``monolog.logger`` タグを追加し、どのチャネルを使用するかを指定することで、このサービス内で logger を使用する際、指定したチャネルにログが出力されるようになります。

コードサンプルを含む詳細は ":ref:`dic_tags-monolog`" を参照して下さい。

.. ":ref:`dic_tags-monolog`" が未翻訳なため、リンク先のサンプルコードを見ることができません。
   応急処置としてここにサンプルコードを置きます。

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: ["@logger"]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('my_service', $definition);


クックブックでより深く
----------------------

* :doc:`/cookbook/logging/monolog`
