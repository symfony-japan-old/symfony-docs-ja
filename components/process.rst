.. index::
   single: Process

Processコンポーネント
=====================

    Processコンポーネントを使うと、サブプロセスでコマンドを実行することができます。

インストール
------------

コンポーネントをインストールする方法は何通りもあります。

* 公式Gitレポジトリ (https://github.com/symfony/Process);
* PEARコマンドでインストール ( `pear.symfony.com/Process`);
* Composerを使ってインストール (Packagistの`symfony/process`).

使用方法
--------

`:class:Symfony\\Component\\Process\\Process` クラスを使うとサブプロセスでコマンドを実行することができます。
::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();
    if (!$process->isSuccessful()) {
        throw new RuntimeException($process->getErrorOutput());
    }

    print $process->getOutput();

``:method::Symfony\\Component\\Process\\Process:run`` メソッドは、コマンド実行時にプラットフォーム間の軽微な差異を考慮してくれます。 

（リモートサーバーへのrsyncのような）長いコマンドを実行する時、``:method::Symfony\\Component\\Process\\Process:run`` メソッドに無名関数を渡すことで、エンドユーザーに対するリアルタイムなフィードバックを表示させることができます。 
::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run(function ($type, $buffer) {
        if ('err' === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

何かPHPコードを独立して実行させたい時は、代わりに``PhpProcess``クラスを使ってください。
::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Hello World'; ?>
    EOF);
    $process->run();

.. versionadded:: 2.1
    ``ProcessBuilder`` は2.1で追加されました。

全てのプラットフォームでよりよく動かしたいなら、代わりに``:class:Symfony\\Component\\Process\\ProcessBuilder``クラスを使うと良いかもしれません::

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder(array('ls', '-lsa'));
    $builder->getProcess()->run();


.. 2012/01/21 77web 115ff31eeb9b9661b5ff39b5be487d201fbd9c74