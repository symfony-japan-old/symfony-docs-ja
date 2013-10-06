.. index::
   single: Doctrine; ORM configuration reference
   single: Configuration reference; Doctrine ORM

.. note::

    * 対象バージョン：DoctrineBundle 1.2 (Symfony 2.2以降)
    * 翻訳更新日：2013/10/06

Doctrine コンフィギュレーションリファレンス
===========================================

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                types:
                    # カスタムタイプのコレクション
                    # 例
                    some_custom_type:
                        class:                Acme\HelloBundle\MyCustomType
                        commented:            true

                connections:
                    default:
                        dbname:               database

                    # 接続名のコレクションも可能 (例 default、conn2 など)
                    default:
                        dbname:               ~
                        host:                 localhost
                        port:                 ~
                        user:                 root
                        password:             ~
                        charset:              ~
                        path:                 ~
                        memory:               ~

                        # MySQLで利用するUNIXソケット
                        unix_socket:          ~

                        # ibm_db2ドライバで持続的接続を利用する場合にTrueにする
                        persistent:           ~

                        # ibm_db2ドライバで利用するプロトコル (何も指定しない場合はデフォルトで TCPIP)
                        protocol:             ~

                        # OracleでSIDの代わりにdbnameをサービス名として利用する場合はTrue
                        service:              ~

                        # oci8ドライバで利用するセッションモード
                        sessionMode:          ~

                        # oci8ドライバでpooledサーバーを利用する場合にTrue
                        pooled:               ~

                        # pdo_sqlsrvドライバ用のMultipleActiveResultSetsのコンフィギュレーション
                        MultipleActiveResultSets:  ~
                        driver:               pdo_mysql
                        platform_service:     ~
                        logging:              %kernel.debug%
                        profiling:            %kernel.debug%
                        driver_class:         ~
                        wrapper_class:        ~
                        options:
                            # オプションの配列
                            key:                  []
                        mapping_types:
                            # マッピングタイプの配列
                            name:                 []
                        slaves:

                            # 名前付けたスレーブ接続のコレクション (例 slave1、slave2)
                            slave1:
                                dbname:               ~
                                host:                 localhost
                                port:                 ~
                                user:                 root
                                password:             ~
                                charset:              ~
                                path:                 ~
                                memory:               ~

                                # MySQLで利用するUNIXソケット
                                unix_socket:          ~

                                # ibm_db2ドライバで持続的接続を利用する場合にTrueにする
                                persistent:           ~

                                # ibm_db2ドライバで利用するプロトコル (何も指定しない場合はデフォルトで TCPIP)
                                protocol:             ~

                                # OracleでSIDの代わりにdbnameをサービス名として利用する場合はTrue
                                service:              ~

                                # oci8ドライバで利用するセッションモード
                                sessionMode:          ~

                                # oci8ドライバでpooledサーバーを利用する場合にTrue
                                pooled:               ~

                                # pdo_sqlsrvドライバ用のMultipleActiveResultSetsのコンフィギュレーション
                                MultipleActiveResultSets:  ~

            orm:
                default_entity_manager:  ~
                auto_generate_proxy_classes:  false
                proxy_dir:            %kernel.cache_dir%/doctrine/orm/Proxies
                proxy_namespace:      Proxies
                # 以下についてはCookbookで "ResolveTargetEntityListener" クラスを検索してください
                resolve_target_entities: []
                entity_managers:
                    # 名前付けたエンティティマネージャーのコレクション (例 some_em、another_em)
                    some_em:
                        query_cache_driver:
                            type:                 array # 必須
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        metadata_cache_driver:
                            type:                 array # 必須
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        result_cache_driver:
                            type:                 array # 必須
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        connection:           ~
                        class_metadata_factory_name:  Doctrine\ORM\Mapping\ClassMetadataFactory
                        default_repository_class:  Doctrine\ORM\EntityRepository
                        auto_mapping:         false
                        hydrators:

                            # ハイドレーター名の配列
                            hydrator_name:                 []
                        mappings:
                            # マッピング名の配列（バンドルごとに指定するならバンドル名）
                            mapping_name:
                                mapping:              true
                                type:                 ~
                                dir:                  ~
                                alias:                ~
                                prefix:               ~
                                is_bundle:            ~
                        dql:
                            # 文字列関数のコレクション
                            string_functions:
                                # 例
                                # test_string: Acme\HelloBundle\DQL\StringFunction

                            # 数値関数のコレクション
                            numeric_functions:
                                # 例
                                # test_numeric: Acme\HelloBundle\DQL\NumericFunction

                            # 日時関数のコレクション
                            datetime_functions:
                                # 例
                                # test_datetime: Acme\HelloBundle\DQL\DatetimeFunction

                        # エンティティマネージャーにSQLフィルタを登録する
                        filters:
                            # フィルタの配列
                            some_filter:
                                class:                ~ # 必須
                                enabled:              false

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection
                        name="default"
                        dbname="database"
                        host="localhost"
                        port="1234"
                        user="user"
                        password="secret"
                        driver="pdo_mysql"
                        driver-class="MyNamespace\MyDriverImpl"
                        path="%kernel.data_dir%/data.sqlite"
                        memory="true"
                        unix-socket="/tmp/mysql.sock"
                        wrapper-class="MyDoctrineDbalConnectionWrapper"
                        charset="UTF8"
                        logging="%kernel.debug%"
                        platform-service="MyOwnDatabasePlatformService"
                    >
                        <doctrine:option key="foo">bar</doctrine:option>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                    <doctrine:connection name="conn1" />
                    <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>

                <doctrine:orm default-entity-manager="default" auto-generate-proxy-classes="false" proxy-namespace="Proxies" proxy-dir="%kernel.cache_dir%/doctrine/orm/Proxies">
                    <doctrine:entity-manager name="default" query-cache-driver="array" result-cache-driver="array" connection="conn1" class-metadata-factory-name="Doctrine\ORM\Mapping\ClassMetadataFactory">
                        <doctrine:metadata-cache-driver type="memcache" host="localhost" port="11211" instance-class="Memcache" class="Doctrine\Common\Cache\MemcacheCache" />
                        <doctrine:mapping name="AcmeHelloBundle" />
                        <doctrine:dql>
                            <doctrine:string-function name="test_string">Acme\HelloBundle\DQL\StringFunction</doctrine:string-function>
                            <doctrine:numeric-function name="test_numeric">Acme\HelloBundle\DQL\NumericFunction</doctrine:numeric-function>
                            <doctrine:datetime-function name="test_datetime">Acme\HelloBundle\DQL\DatetimeFunction</doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>
                    <doctrine:entity-manager name="em2" connection="conn2" metadata-cache-driver="apc">
                        <doctrine:mapping
                            name="DoctrineExtensions"
                            type="xml"
                            dir="%kernel.root_dir%/../vendor/gedmo/doctrine-extensions/lib/DoctrineExtensions/Entity"
                            prefix="DoctrineExtensions\Entity"
                            alias="DExt"
                        />
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>

コンフィギュレーションの概要
----------------------------

以下のコンフィギュレーション例は、ORMが利用するすべてのデフォルトコンフィギュレーションです。

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            # Symfony Standard ディストリビューションでは、次の設定はデバッグモードで true に上書きされます。他の場合は false です。
            auto_generate_proxy_classes: false
            proxy_namespace: Proxies
            proxy_dir: "%kernel.cache_dir%/doctrine/orm/Proxies"
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

特定のクラスを変更可能なコンフィギュレーションオプションが多数ありますが、通常は変更する必要はありません。

キャッシュドライバー
~~~~~~~~~~~~~~~~~~~~

キャッシュドライバーには "array"、"apc"、"memcache"、"memcached"、"xcache"、"service" の配列を指定できます。

キャッシュコンフィギュレーション例を次に示します。

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver:
                type: service
                id: my_doctrine_common_cache_service
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

マッピングコンフィギュレーション
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ORMで、マッピングするすべてのエンティティに対して明示的にマッピングを定義する必要があります。
設定可能なコンフィギュレーションオプションがいくつからあります。マッピングについては、以下のオプションがあります。

* ``type`` には、\ ``annotation``\ 、\ ``xml``\ 、\ ``yml``\ 、\ ``php``\ 、\ ``staticphp`` のどれかを指定します。
  マッピング定義に利用するメタデータの種類を指定します。

* ``dir`` には、ドライバに応じてマッピング定義ファイルやエンティティクラスファイルのあるパスを指定します。
  相対パスを指定する場合は、バンドルのルートディレクトリを起点としてください。
  （マッピングがバンドル名の場合の動作）このオプションで絶対パスを使いたい場合は、DICにあるカーネルパラメータからパスのプレフィックスを取得してください (例 %kernel.root_dir%)。

* ``prefix`` には、このマッピングに共通で利用する名前空間プレフィックスを指定します。
  マッピング定義間でプレフィックスが競合しないようにしてください。プレフィックスが競合すると、Doctrineがエンティティを見つけられなくなります。
  デフォルトでは、バンドルの名前空間の ``Entity`` になります。
  たとえば、\ ``AcmeHelloBundle`` のプレフィックスは ``Acme\HelloBundle\Entity`` になります。

* ``alias`` にエンティティの名前空間のエイリアスを指定すると、短い単純な名前をDQLクエリやリポジトリアクセスに利用できます。
  バンドルを利用している場合は、デフォルトでバンドル名になります。

* ``is_bundle`` オプションは ``dir`` に関連します。デフォルトは ``true`` で、\ ``dir`` に設定したパスは ``file_exists()`` チェックで false を返しますが、相対的に評価されます。このオプションを ``false`` にすると、ファイルの存在チェックが有効になります。
  ``dir`` に絶対パスが指定され、メタデータファイルがバンドル外のディレクトリにある場合に使います。

.. index::
    single: Configuration; Doctrine DBAL
    single: Doctrine; DBAL configuration

.. _`reference-dbal-configuration`:

Doctrine DBAL コンフィギュレーション
------------------------------------

DoctrineBundle では、Doctrine ドライバで有効なすべてのデフォルト値が利用できます。
Symfony 向けに XML または YAML 形式で記述できるようになっています。
詳細は `DBAL ドキュメント`_ を参照してください。
コンフィギュレーションで利用可能なすべてのキーは、次のとおりです。

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                dbname:               database
                host:                 localhost
                port:                 1234
                user:                 user
                password:             secret
                driver:               pdo_mysql
                # DBAL driverClass オプション
                driver_class:         MyNamespace\MyDriverImpl
                # DBAL driverOptions オプション
                options:
                    foo: bar
                path:                 "%kernel.data_dir%/data.sqlite"
                memory:               true
                unix_socket:          /tmp/mysql.sock
                # DBAL wrapperClass オプション
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              "%kernel.debug%"
                platform_service:     MyOwnDatabasePlatformService
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HelloBundle\MyCustomType
                # DBAL keepSlave オプション
                keep_slave:           false

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="database"
                host="localhost"
                port="1234"
                user="user"
                password="secret"
                driver="pdo_mysql"
                driver-class="MyNamespace\MyDriverImpl"
                path="%kernel.data_dir%/data.sqlite"
                memory="true"
                unix-socket="/tmp/mysql.sock"
                wrapper-class="MyDoctrineDbalConnectionWrapper"
                charset="UTF8"
                logging="%kernel.debug%"
                platform-service="MyOwnDatabasePlatformService"
            >
                <doctrine:option key="foo">bar</doctrine:option>
                <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
            </doctrine:dbal>
        </doctrine:config>

YAML で複数のコネクションを構成したい場合は、\ ``connections`` キー以下に一意な接続名で次のように設定します。

.. code-block:: yaml

    doctrine:
        dbal:
            default_connection:       default
            connections:
                default:
                    dbname:           Symfony2
                    user:             root
                    password:         null
                    host:             localhost
                customer:
                    dbname:           customer
                    user:             root
                    password:         null
                    host:             localhost

``database_connection`` サービスは常に *デフォルト* コネクションを参照します。
これは、最初に定義されたコネクションか、または ``default_connection`` パラメータにより指定したコネクションです。

各コネクションは、サービス名 ``doctrine.dbal.[name]_connection`` でアクセスすることもできます。
``[name]`` の部分がコネクション名になります。

.. _DBAL ドキュメント: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/reference/configuration.html

.. 2013/10/06 hidenorigoto 5c401290e2349bdcaef324acb0f25ec0a4f45129

