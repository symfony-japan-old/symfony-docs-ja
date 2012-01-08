.. index::
   single: Debugging

デバッグするための開発環境に最適化する方法
==========================================

ローカルマシーンで Symfony のプロジェクトを作る際には、 ``dev`` 環境を使用するべきです(``app_dev.php`` フロントコントローラ)。この環境設定は、次の主な２つの目的に最適化されています。:

 * 何か問題があった際に、開発者に正確なフィードバックを与えること(ウェブデバッグツールバー、例外ページ、プロファイラ)。

 * プロジェクトをデプロイするときの問題を避けるために、本番環境に極力近づけること。

.. _cookbook-debugging-disable-bootstrap:

ブートストラップファイルとクラスキャッシュを無効にする
------------------------------------------------------

本番環境を極力速くするために、 Symfony はリクエスト毎にプロジェクトが必要とする PHP クラスの集合を含んだ大きな PHP ファイルをキャッシュに作成します。しかし、このキャッシュの使用によって IDE やデバッガを混乱させてしまうことがあります。この記事では、 Symfony のクラスを含むデバッグコードが必要な際に、より簡単にキャッシュのメカニズムを調整する方法を紹介します。

デフォルトでは、フロントコントローラの ``app_dev.php`` は次のようになっています。
::

    // ...

    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

デバッガをより充実させるために、次のように ``loadClassCache()`` メソッドを呼び、 ``require`` の一行を置き換えて、全ての PHP クラスキャッシュを無効にします。
::
    // ...

    // require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once __DIR__.'/../app/autoload.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    // $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

.. tip::

    PHP キャッシュを無効にしたら、デバッグの後に元に戻すことを忘れないでください。

クラスのいくつかが、異なる場所に格納されていることを好まない IDE もあります。この問題を避けるために、 IDE に PHP キャッシュファイルを無視するように伝えるか、もしくは、次のように Symfony からエクステンションを変更してください。
::
    $kernel->loadClassCache('classes', '.php.cache');

.. 2011/10/30 ganchiku 123515c0936079198c55693e9151f257dbb02e10

