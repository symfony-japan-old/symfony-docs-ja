メールをスプールする方法
==================

``SwiftmailerBundle`` を使用してSymfony2 のアプリケーションからメールを送信する際に、デフォルトでは、すぐにメールを送信する設定になっています。しかし、 ``Swfitmailer`` とメールトランスポートのコミュニケーションによるパフォーマンス低下は避けた方が良いときもあるでしょう。次のページをロードするのに、メール送信に時間がかかってしまえば、ユーザは待たなければならないですから。この問題を避けるために、メールを直接送信するのではなく、 "spool" を選択することができます。つまり、 ``Swiftmailer`` がメールを送信しようとするのではなく、ファイルなどにメッセージを保存します。そして、他のプロセスがスプールを読み、メールの送信の面倒をみます。現時点の ``Swfitmailer`` では、ファイルへのスプールのみを対応しています。

スプールを使用するために、コンフィギュレーションを次のようにしてください。
:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            # ...
            spool:
                type: file
                path: /path/to/spool

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config>
             <swiftmailer:spool
                 type="file"
                 path="/path/to/spool" />
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
             // ...
            'spool' => array(
                'type' => 'file',
                'path' => '/path/to/spool',
            )
        ));

.. tip::

    スプールをあなたのプロジェクトのディレクトリ内に保存するには、プロジェクトのルートへの参照ができる `%kernel.root_dir%` パラメターを使用してください。
    :

    .. code-block:: yaml

        path: %kernel.root_dir%/spool

これで、あなたのアプリケーションがメールを「送信」すれば、実際に送信するのではなく、スプールに追加されるようになりました。スプールからのメッセージの送信を非同期にすることができるようになりました。そして、次のようにスプール内のメッセージを送信するコンソールコマンドがあります。
:

.. code-block:: bash

    php app/console swiftmailer:spool:send

オプションとして送信するメッセージの数を指定することができます。
:

.. code-block:: bash

    php app/console swiftmailer:spool:send --message-limit=10

また、秒ごとの送信数を指定することもできます。
:

.. code-block:: bash

    php app/console swiftmailer:spool:send --time-limit=10

実際には、このコマンドを主導で実行したいとは思わないでしょう。その際には、 cron ジョブやスケジュールタスクなどを使用してコマンドをトリガーし、定期間隔による実行をしてください。

.. 2011/10/31 ganchiku c3ffbfba2c139ece7b0160b6cb8f2b3d6fb93482

