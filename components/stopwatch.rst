.. index::
   single: Stopwatch
   single: Components; Stopwatch

.. note::

    * 対象バージョン：2.3 (2.2以降)
    * 翻訳更新日：2013/05/16

Stopwatchコンポーネント
=======================

    Stopwatchコンポーネントはコードをプロファイルする方法を提供します。

.. versionadded:: 2.2
    Stopwatchコンポーネントは、Symfony2.2から追加されました。以前は ``Stopwatch`` クラスは ``HttpKernel`` コンポーネントに含まれていました(Symfony2.1以降)。

インストール
------------

コンポーネントのインストール方法は二通りです。

* 公式Gitレポジトリ (https://github.com/symfony/Stopwatch);
* :doc:`Composer</components/using_components>` (`Packagist`_ の ``symfony/stopwatch``)

使い方
-----

Stopwatchコンポーネントは任意のコードの実行時間を測る簡単で一貫性のある方法を提供します。もうマイクロ秒を自力で解析する必要はないのです。 代わりに、 :class:`Symfony\\Component\\Stopwatch\\Stopwatch` クラスを使ってください。::

    use Symfony\Component\Stopwatch\Stopwatch;

    $stopwatch = new Stopwatch();
    // 'eventName'というイベントを開始
    $stopwatch->start('eventName');
    // ... ここに何かのコード
    $event = $stopwatch->stop('eventName');

イベントにカテゴリ名をつけることもできます::

    $stopwatch->start('eventName', 'categoryName');

カテゴリはイベントに対するタグのようなものと考えてください。例えば、Symfony Profilter tookはカテゴリをイベントの色分けに使用しています。

ピリオド
-------

現実の世界と同じように、ストップウォッチには2つのボタンがあります。ストップウォッチをスタート・ストップするボタンと、ラップタイムを測るボタンです。
ちょうど :method:`Symfony\\Component\\Stopwatch\\Stopwatch::lap`` メソッドの動作と同じです。::

    $stopwatch = new Stopwatch();
    // 'foo'というイベントを開始
    $stopwatch->start('foo');
    // ... ここに何かのコード
    $stopwatch->lap('foo');
    // ... ここに何かのコード
    $stopwatch->lap('foo');
    // ... ここに何か別のコード
    $event = $stopwatch->stop('foo');

ラップの情報は"periods"としてイベント内に保存されます。ラップ情報を取得するには次のように呼び出してください。::

    $event->getPeriods();

イベントオブジェクトからは、ピリオドだけでなく他の情報も取得することができます。
例えば::

    $event->getCategory();      // イベントのカテゴリを取得
    $event->getOrigin();        // イベントの開始時刻をマイクロ秒まで取得
    $event->ensureStopped();    // まだ停止していないイベントを全て停止
    $event->getStartTime();     // 一番最初のピリオドの開始時刻を取得
    $event->getEndTime();       // 一番最後のピリオドの終了時刻を取得
    $event->getDuration();      // 全てのピリオドを通しての所要時間を取得
    $event->getMemory();        // 全てのピリオドを通してのメモリの最大使用量を取得

セクション
--------

セクションではタイムラインを論理的にグループ分けすることができます。Symfony Profilter toolを見ると、Symfonyがフレームワークのライフサイクルを視覚化するのにセクションをうまく利用していることがわかります。下記は基本的なセクションの使い方です。::

    $stopwatch = new Stopwatch();

    $stopwatch->openSection();
    $stopwatch->start('parsing_config_file', 'filesystem_operations');
    $stopwatch->stopSection('routing');

    $events = $stopwatch->getSectionEvents('routing');

:method:`Symfony\\Component\\Stopwatch\\Stopwatch::openSection`` メソッドを使うと、一度閉じたセクションを指定して再度開くことができます。::

    $stopwatch->openSection('routing');
    $stopwatch->start('building_config_tree');
    $stopwatch->stopSection('routing');

.. _Packagist: https://packagist.org/packages/symfony/stopwatch


.. 2013/05/16 77web 65399256d8d3befabee05b1b7f4bce25cfb5ac1b