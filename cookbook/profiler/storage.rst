.. note::

 * 対象バージョン：2.6
 * 翻訳更新日：2014/12/01

.. index::
    single: Profiling; Storage Configuration

Profiler のストレージを変更する
==============================

デフォルトでは Profiler はキャッシュディレクトリ内のファイルに集積されたデータ蓄積します。
このストレージは ``dsn``, ``username``, ``password``, ``lifetime`` を介して調整が可能です。
例えば、lifetime が１時間として MySQL を Profiler のストレージとして仕様する設定は以下の通りです:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
        profiler:
            dsn:      "mysql:host=localhost;dbname=%database_name%"
            username: "%database_user%"
            password: "%database_password%"
            lifetime: 3600

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <framework:profiler
                    dsn="mysql:host=localhost;dbname=%database_name%"
                    username="%database_user%"
                    password="%database_password%"
                    lifetime="3600"
                />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php

        // ...
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'dsn'      => 'mysql:host=localhost;dbname=%database_name%',
                'username' => '%database_user',
                'password' => '%database_password%',
                'lifetime' => 3600,
            ),
        ));

:doc:`HttpKernel component </components/http_kernel/introduction>`
は現在、以下の Profiler 用のストレージをサポートしています。

* :class:`Symfony\\Component\\HttpKernel\\Profiler\\FileProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MemcachedProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MemcacheProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MongoDbProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MysqlProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\RedisProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\SqliteProfilerStorage`
