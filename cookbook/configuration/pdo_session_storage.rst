.. index::
   single: Session; Database Storage

PdoSessionStorage を使用してデータベースにセッションを格納する方法
==================================================================

Symfony2 のデフォルトのセッションストレージは、ファイルにセッションの値を記録することでした。中規模から大規模なウェブサイトでは、セッションの値をファイルに格納するのではなく、データベースに格納をします。それは、データベースの方が簡単に扱うことができ、複数のウェブサーバの構成にスケールアウトすることができるからです。

Symfony2 はセッションストレージにデータベースを使用するクラスをビルトインしています。クラス名は、 :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\PdoSessionStorage` です。このクラスを使用するには、 ``config.yml`` (もしくは選択したコンフィギュレーションのフォーマット)のパラメターを数個変更するだけです。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                storage_id:     session.storage.pdo

        parameters:
            pdo.db_options:
                db_table:    session
                db_id_col:   session_id
                db_data_col: session_value
                db_time_col: session_time

        services:
            pdo:
                class: PDO
                arguments:
                    dsn:      "mysql:dbname=mydatabase"
                    user:     myuser
                    password: mypassword

            session.storage.pdo:
                class:     Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage
                arguments: [@pdo, %session.storage.options%, %pdo.db_options%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session storage-id="session.storage.pdo" lifetime="3600" auto-start="true"/>
        </framework:config>

        <parameters>
            <parameter key="pdo.db_options" type="collection">
                <parameter key="db_table">session</parameter>
                <parameter key="db_id_col">session_id</parameter>
                <parameter key="db_data_col">session_value</parameter>
                <parameter key="db_time_col">session_time</parameter>
            </parameter>
        </parameters>

        <services>
            <service id="pdo" class="PDO">
                <argument>mysql:dbname=mydatabase</argument>
                <argument>myuser</argument>
                <argument>mypassword</argument>
            </service>

            <service id="session.storage.pdo" class="Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage">
                <argument type="service" id="pdo" />
                <argument>%session.storage.options%</argument>
                <argument>%pdo.db_options%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.yml
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            // ...
            'session' => array(
                // ...
                'storage_id' => 'session.storage.pdo',
            ),
        ));

        $container->setParameter('pdo.db_options', array(
            'db_table'      => 'session',
            'db_id_col'     => 'session_id',
            'db_data_col'   => 'session_value',
            'db_time_col'   => 'session_time'
        ));

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=mydatabase',
            'myuser',
            'mypassword',
        ));
        $container->setDefinition('pdo', $pdoDefinition);

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage', array(
            new Reference('pdo'),
            '%session.storage.options%',
            '%pdo.db_options%',
        ));
        $container->setDefinition('session.storage.pdo', $storageDefinition);

* ``db_table``: データベースのセッションテーブル名
* ``db_id_col``: セッションテーブルの id カラムの名前 (VARCHAR(255) または、より大きくしてください)
* ``db_data_col``: セッションテーブルの value カラムの名前 (TEXT または CLOG)
* ``db_time_col``: セッションテーブルの time カラムの名前 (INTEGER)

データベース接続情報を共有する
------------------------------

今回指定したコンフィギュレーションでは、データベース接続の設定に、セッションストレージの接続のみ定義しています。セッションデータのための独立したデータベースを使用するならば、これで問題がありません。

しかし、他のプロジェクトデータと同じデータベースでセッションデータを格納には、 parameter.ini に定義されたデータベースに関連するパラメターを参照して接続設定を使用することができます。

.. configuration-block::

    .. code-block:: yaml

        pdo:
            class: PDO
            arguments:
                - "mysql:dbname=%database_name%"
                - %database_user%
                - %database_password%

    .. code-block:: xml

        <service id="pdo" class="PDO">
            <argument>mysql:dbname=%database_name%</argument>
            <argument>%database_user%</argument>
            <argument>%database_password%</argument>
        </service>

    .. code-block:: xml

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=%database_name%',
            '%database_user%',
            '%database_password%',
        ));

SQL 構文の例
------------

MySQL
~~~~~

必要なデータベースのテーブルを作成するSQL 構文は以下のようになります(MySQL)。

.. code-block:: sql

    CREATE TABLE `session` (
        `session_id` varchar(255) NOT NULL,
        `session_value` text NOT NULL,
        `session_time` int(11) NOT NULL,
        PRIMARY KEY (`session_id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

PostgreSQL
~~~~~~~~~~

PostgreSQL では、構文は以下のようになります。

.. code-block:: sql

    CREATE TABLE session (
        session_id character varying(255) NOT NULL,
        session_value text NOT NULL,
        session_time integer NOT NULL,
        CONSTRAINT session_pkey PRIMARY KEY (session_id)
    );

.. 2011/12/27 ganchiku f46507a719ea01269df44478c2afd8d229daa008

