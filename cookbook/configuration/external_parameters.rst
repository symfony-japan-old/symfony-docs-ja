.. index::
   single: Environments; External Parameters

サービスコンテナで外部パラメターをセットする方法
================================================

:doc:`/cookbook/configuration/environments` の章では、アプリケーションのコンフィギュレーションの運営方法について学びました。なたのプロジェクトのコードの外部に証明書 (credentials) を格納することで恩恵があるときもあるでしょう。例えば、データベースに関する設定です。 Symfony のサービスコンテナは柔軟なので、そういったことも簡単にできます。

環境変数
--------

Symfony は、 ``SYMFONY__`` の接頭辞の付いたあらゆる環境変数を読み取り、サービスコンテナのパラメータとしてセットすることができます。環境変数名では、ピリオドを使用することできないため、アンダースコア２つを使用しており、 Symfony 内で、ピリオドに置換されます。

Apache を使用してるのであれば、次の ``VirtualHost`` の設定を使用して、環境変数をセットすることができます

.. code-block:: apache

    <VirtualHost *:80>
        ServerName      Symfony2
        DocumentRoot    "/path/to/symfony_2_app/web"
        DirectoryIndex  index.php index.html
        SetEnv          SYMFONY__DATABASE__USER user
        SetEnv          SYMFONY__DATABASE__PASSWORD secret

        <Directory "/path/to/my_symfony_2_app/web">
            AllowOverride All
            Allow from All
        </Directory>
    </VirtualHost>

.. note::

    上記の例は、  `SetEnv` ディレクティブを使用して Apache の設定をしています。環境変数のセットを支援する全てのウェブサーバであれば、このように使用することができます。

    また、 Apache からではなく、コンソールから使用できるようにするために、シェル変数としてこれらの設定をエクスポートする必要があります。 Unix の場合であれば、次を実行してください。
    
    .. code-block:: bash
    
        export SYMFONY__DATABASE__USER=user
        export SYMFONY__DATABASE__PASSWORD=secret

これで環境変数を宣言したので、環境変数は、 PHP のグローバル変数 ``$_SERVER`` に入ります。そして Symfony は、自動的に ``$_SERVER`` 変数を ``SYMFONY__`` の接頭辞を付けてサービスコンテナのパラメターとしてセットします。

これで必要なときに、これらのパラメターを参照することができます。

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                driver    pdo_mysql
                dbname:   symfony2_project
                user:     %database.user%
                password: %database.password%

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                driver="pdo_mysql"
                dbname="symfony2_projet"
                user="%database.user%"
                password="%database.password%"
            />
        </doctrine:config>

    .. code-block:: php

        $container->loadFromExtension('doctrine', array('dbal' => array(
            'driver'   => 'pdo_mysql',
            'dbname'   => 'symfony2_project',
            'user'     => '%database.user%',
            'password' => '%database.password%',
        ));

定数
----

コンテナは、 PHP の定数もパラメターとしてセットできます。この機能のアドバンテージを享受するには、定数の名前をパラメターキーとしてマップし、 ``constant`` としてそのタイプを定義します。

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        >

            <parameters>
                <parameter key="global.constant.value" type="constant">GLOBAL_CONSTANT</parameter>
                <parameter key="my_class.constant.value" type="constant">My_Class::CONSTANT_NAME</parameter>
            </parameters>
        </container>

.. note::

    これは XML のコンフィギュレーションのみで動作します。 XML を使用して *いなければ* 、次のようにこの機能のアドバンテージを享受するための XML をインポートするだけです。
    
    .. code-block:: yaml
    
        // app/config/config.yml
        imports:
            - { resource: parameters.xml }

その他のコンフィギュレーション
------------------------------

``imports`` ディレクティブは、他の場所で格納されたパラメターを参照することができます。 PHP ファイルをインポートすることができるので、コンテナの中で必要になったもの全てを加えることができ、柔軟です。次の例では、 ``parameters.php`` という名前のファイルをインポートいています。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.php }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.php" />
        </imports>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.php');

.. note::

    リソースファイルは、 PHP, XML, YAML, INI, そしてクロージャリソースなどたくさんの種類を使用することができ、全て ``imports`` ディレクティブでサポートしています。

``parameters.php`` では、サービスコンテナにセットしたいパラメターを指定します。非標準なフォーマットのコンフィギュレーションをインポートする際に便利です。下記の例では、 Drupal のデータベースの設定を Symfony のサービスコンテナにインクルードしています。

.. code-block:: php

    // app/config/parameters.php

    include_once('/path/to/drupal/sites/default/settings.php');
    $container->setParameter('drupal.database.url', $db_url);

.. _`SetEnv`: http://httpd.apache.org/docs/current/env.html

.. 2011/11/28 ganchiku c58e7a35072cd8ce6b711dff2acb740b2aef4cbe

