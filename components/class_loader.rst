.. index::
   pair: Autoloader; Configuration

ClassLoaderコンポーネント
=========================

    ClassLoaderコンポーネントは、標準PHP規約に従っているプロジェクトのクラスを自動的に読みこみます。

未定義のクラスが使われている時、PHPはクラス定義のあるファイルを読みこむのにオートロードの仕組みを使います。 Symfony2は「汎用的」に使える自動読み込み機構を提供します。つまり、下記の規約のいずれかに従ってクラス定義を書いたファイルを読みこむことができるのです:

* PHP5.3用の `相互標準規約`_ による名前空間及びクラス命名規則

* `PEAR`_ のクラス命名規則

プロジェクトで利用するクラスやサードパーティのライブラリがこのような基準に従っているならば、Symfony2のオートローダが役立つでしょう。

Installation
------------

You can install the component in many different ways:

* Use the official Git repository (https://github.com/symfony/ClassLoader);
* Install it via PEAR ( `pear.symfony.com/ClassLoader`);
* Install it via Composer (`symfony/class-loader` on Packagist).

Usage
-----

.. versionadded:: 2.1
   The ``useIncludePath`` method was added in Symfony 2.1.

Registering the :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader`
autoloader is straightforward::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();

    // You can search the include_path as a last resort.
    $loader->useIncludePath(true);

    $loader->register();

For minor performance gains class paths can be cached in memory using APC by
registering the :class:`Symfony\\Component\\ClassLoader\\ApcUniversalClassLoader`::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once '/path/to/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('apc.prefix.');
    $loader->register();

The autoloader is useful only if you add some libraries to autoload.

.. note::

    The autoloader is automatically registered in a Symfony2 application (see
    ``app/autoload.php``).

If the classes to autoload use namespaces, use the
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespace`
or
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespaces`
methods::

    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/src',
        'Monolog' => __DIR__.'/../vendor/monolog/src',
    ));

    $loader->register();

For classes that follow the PEAR naming convention, use the
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefix`
or
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefixes`
methods::

    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/lib',
    ));

    $loader->register();

.. note::

    Some libraries also require their root path be registered in the PHP
    include path (``set_include_path()``).

Classes from a sub-namespace or a sub-hierarchy of PEAR classes can be looked
for in a location list to ease the vendoring of a sub-set of classes for large
projects::

    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine-common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine-dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/lib',
    ));

    $loader->register();

In this example, if you try to use a class in the ``Doctrine\Common`` namespace
or one of its children, the autoloader will first look for the class under the
``doctrine-common`` directory, and it will then fallback to the default
``Doctrine`` directory (the last one configured) if not found, before giving up.
The order of the registrations is significant in this case.

.. _standards: http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _PEAR:      http://pear.php.net/manual/en/standards.php
