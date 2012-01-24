.. index::
   pair: Doctrine; DBAL

Doctrine の DBAL レイヤーの使用方法
===================================

.. note::

    この記事は、 Doctrine の DBAL レイヤーに関するものです。通常は、より高いレベルの Doctrine ORM レイヤーを使用します。 Doctrine ORM レイヤーは、データベースを実際にコミュニケートするシーンの背後で DBAL を使用しています。 Doctrine ORM に関する詳細は、 ":doc:`/book/doctrine`" を参照してください。

`Doctrine`_ データベースアブストラクションレイヤー (DBAL) は、 `PDO`_ の上の抽象レイヤーで、ほとんどの人気のあるリレーショナルデータベースとコミュニケートする直感的で柔軟な API を提供します。つまり、 DBAL ライブラリは、クエリーを実行するなどデータベースの処理を簡単にさせてくれます。

.. tip::

    公式の Doctrine のドキュメント `DBAL Documentation`_ を読むと、 Doctrine の DBAL ライブラリの詳細や実現可能なことを全て学ぶことができます。

まず、データベース接続のパラメータを設定してください。
:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   pdo_mysql
                dbname:   Symfony2
                user:     root
                password: null
                charset:  UTF8

    .. code-block:: xml

        // app/config/config.xml
        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="Symfony2"
                user="root"
                password="null"
                driver="pdo_mysql"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'    => 'pdo_mysql',
                'dbname'    => 'Symfony2',
                'user'      => 'root',
                'password'  => null,
            ),
        ));

完全な DBAL コンフィギュレーションのオプションは、 :ref:`reference-dbal-configuration` を参照してください。

これで、 ``database_connection`` サービスを通して Doctrine DBAL 接続にアクセスできるようになりました。
:

.. code-block:: php

    class UserController extends Controller
    {
        public function indexAction()
        {
            $conn = $this->get('database_connection');
            $users = $conn->fetchAll('SELECT * FROM users');

            // ...
        }
    }

カスタムマップタイプを登録する
------------------------------

Symfony のコンフィギュレーションを通して、カスタムマップタイプを登録することができます。これは、設定された全ての接続に追加されます。カスタムマップタイプの詳細は、 Doctrine のドキュメントの `Custom Mapping Types`_ セクションを参照してください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                types:
                    custom_first: Acme\HelloBundle\Type\CustomFirst
                    custom_second: Acme\HelloBundle\Type\CustomSecond

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'connections' => array(
                    'default' => array(
                        'mapping_types' => array(
                            'enum'  => 'string',
                        ),
                    ),
                ),
            ),
        ));

スキーマツールでカスタムマップタイプを登録する
----------------------------------------------

スキーマツールは、スキーマと比較してデータベースを調べるために使用されます。このタスクを行うには、それぞれのデータベースタイプにどのマップタイプが必要か知ってる必要があります。新しく追加する際には、コンフィギュレーションを通して登録します。

デフォルトでは、 DBAL でサポートされていない ENUM タイプを ``string`` マップタイプにマップしましょう。
:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                connection:
                    default:
                        // Other connections parameters
                        mapping_types:
                            enum: string

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                    <doctrine:type name="custom_first" class="Acme\HelloBundle\Type\CustomFirst" />
                    <doctrine:type name="custom_second" class="Acme\HelloBundle\Type\CustomSecond" />
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'types' => array(
                    'custom_first'  => 'Acme\HelloBundle\Type\CustomFirst',
                    'custom_second' => 'Acme\HelloBundle\Type\CustomSecond',
                ),
            ),
        ));

.. _`PDO`:           http://www.php.net/pdo
.. _`Doctrine`:      http://www.doctrine-project.org/projects/dbal/2.0/docs/en
.. _`DBAL Documentation`: http://www.doctrine-project.org/projects/dbal/2.0/docs/en
.. _`Custom Mapping Types`: http://www.doctrine-project.org/docs/dbal/2.0/en/reference/types.html#custom-mapping-types

.. 2011/11/01 ganchiku 18d712395a276b75107c19e4b8335be99e7000cc

