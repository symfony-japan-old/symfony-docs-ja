.. index::
   pair: Autoloader; Configuration

クラスのオートローディングの方法
================================

未定のクラスを使用すると常に、 PHP は、クラオートローディングのメカニズムを使用して、クラス定義をしてあるファイルをロードしてしようとします。 Symfony2 は、 "universal autoloader" を提供しており、次のいずれかの慣習に従って実装されたファイルからクラスを読み込むことができます。:

* PHP 5.3 のネームスペースとクラス名による、技術的な相互運用標準(`standards`_ )

* `PEAR`_ のクラスの命名規則

あなたのクラスとサードパーティのライブラリが、この標準に従っていれば、 Symfony2 のオートローダーのみで全て解決できます。

使用方法
--------

.. versionadded:: 2.1
   ``useIncludePath`` メソッドは Symfony2.1 で加えられます。

:class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` オートローダーの登録は、簡単です

::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();

    // You can search the include_path as a last resort.
    $loader->useIncludePath(true);

    $loader->register();

 :class:`Symfony\\Component\\ClassLoader\\ApcUniversalClassLoader` クラスを登録して、APC を使ったメモリ上のクラスパスのキャッシュ化する小さいパフォーマンス改善もできます

::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once '/path/to/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('apc.prefix.');
    $loader->register();

オートロードすべきライブラリがある際には、オートローダーは便利です。

.. note::

    Symfony2 のアプリケーションでは、オートローダーは自動的に登録されます。 ``app/autoload.php`` を参照してください。

ネームスペースを使用しているクラスをオートロードするには
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespace` メソッドを使用する
もしくは、
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespaces` メソッドを使用してください。

::

    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/src',
        'Monolog' => __DIR__.'/../vendor/monolog/src',
    ));

PEAR の命名規則に従ったクラスをオートロードするには
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefix` メソッドを使用する
もしくは、
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefixes` メソッドを使用してください。

::

    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/lib',
    ));

.. note::

    PHP の include pathにライブラリのルートパスの登録が必要なライブラリもあります(``set_include_path()``)。

PEAR のクラスのサブネームスペースや下の階層にあるクラスは、大きなプロジェクトのクラスの集合のベンダーとしたディレクトリのリストより見つけることができます。

::

    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine-common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine-dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/lib',
    ));

この例では、 ``Doctrine\Common`` ネームスペース内のクラス、もしくはその下のクラスを使用するには、オートーローダーは、まず ``doctrine-common`` ディレクトリの下を探します。見つからなければ、探すのを諦める前に、一番下に設定してある デフォルトの ``Doctrine`` ディレクトリを探します。この例においては、登録の順番は、重要です。

.. _standards: http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _PEAR:      http://pear.php.net/manual/en/standards.php

.. 2011/10/28 ganchiku e8ea59aa073139915feb38ef2471c16045c0a7bd

