.. index::
   single: Debug
   single: Components; Debug


.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/9/27

Debugコンポーネント
===================

    DebugコンポーネントはPHPのデバッグを手助けします。

インストール
------------

Debugコンポーネントは以下の2つの方法でインストールができます。

* 公式Gitリポジトリ (https://github.com/symfony/Debug);
* Composerを使ってインストール (`Packagist`の ``symfony/debug``).

使い方
------

| DebugコンポーネントはいくつかのPHPのデバッグに役立つのツールで構成されています。
| 以下のコードを実行するとすべての機能が有効になります。

    use Symfony\Component\Debug\Debug;

    Debug::enable();

| :method:`Symfony\\Component\\Debug\\Debug::enable` メソッドはPHPのエラーおよび例外を処理します。
| プロジェクトで :doc:`ClassLoaderコンポーネント</components/class_loader>` を使用している場合、これらもデバッグモードとして動作するようになります。

各ツールの詳細については次のセクションをご覧ください。

.. caution::

    デバッグツールはサイトへの攻撃の手助けにも繋がるため、運用環境で使用するべきではありません。

エラーハンドラを有効にする
--------------------------

| :class:`Symfony\\Component\\Debug\\ErrorHandler` クラスは、PHPのエラーを検出し、例外に変換してスローします。(:class:`ContextErrorException` 、Fatalエラーの場合は:class:`Symfony\\Component\\Debug\\Exception\\FatalErrorException`)
| 有効にするには次のコードを実行します。

    use Symfony\Component\Debug\ErrorHandler;

    ErrorHandler::register();

例外ハンドラを有効にする
------------------------

| :class:`Symfony\\Component\\Debug\\ExceptionHandler` クラスは、捕捉されなかった例外を検出し、デフォルトやXDebugよりも見やすく、役立つデバッグの情報を画面に表示します。
| 有効にするには次のコードを実行します。

    use Symfony\Component\Debug\ExceptionHandler;

    ExceptionHandler::register();

.. note::

| プロジェクトで :doc:`HttpFoundation component </components/http_foundation/introduction>` を使用している場合、例外ハンドラはResponseオブジェクトを使用します。使用していない場合はPHP標準の処理で出力を行います。

.. _Packagist: https://packagist.org/packages/symfony/debug

.. 2013/09/27 issei-m 1dcfe8df2c3a0edd28bc39968ce531e7e378c728
