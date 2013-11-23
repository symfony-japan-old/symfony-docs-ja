.. index::
    single: Console; CLI
    single: Components; Console

Console コンポーネント
=====================

    Consoleコンポーネントを使うと、美しくてテスト可能なコマンドラインインターフェイスを簡単に作ることができます。

Console コンポーネントを使って、コマンドライン用コマンドを作成することが出来ます。
コンソールコマンドは、cronジョブやインポートなどのバッチジョブなどの自動更新タスクに使用されます。

インストール
------------

Consoleコンポーネントのインストール方法は二通りあります。

* :doc:`Composer を利用する </components/using_components>` (``symfony/console`` on `Packagist`_);
* 公式 Git リポジトリを利用する (https://github.com/symfony/Console).

.. note::

     Windows は、標準では ANSI カラーをサポートしていません。
     そのため、Windows環境上では、 Console コンポーネントがそれを検知し、ANSI カラーでの出力を無効にします。
     ただし、ANSI ドライバーが設定されていない Windows 環境上において、
     作成したコマンドが ANSI カラーシーケンスを吐くスクリプトを呼び出した場合は、
     エスケープ文字としてそのまま表示されます。

     Windows で ANSI カラーをサポートするためには、 `ANSICON`_ をインストールして下さい。


ベーシックなコマンドの作成
--------------------------

コマンドラインから挨拶してくれる、コンソールコマンドを作成してみましょう。
``GreetCommand.php`` ファイルを作成し、以下のコードを追加します。::

    namespace Acme\DemoBundle\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends Command
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Greet someone')
                ->addArgument(
                    'name',
                    InputArgument::OPTIONAL,
                    'Who do you want to greet?'
                )
                ->addOption(
                   'yell',
                   null,
                   InputOption::VALUE_NONE,
                   'If set, the task will yell in uppercase letters'
                )
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Hello '.$name;
            } else {
                $text = 'Hello';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

コマンドラインで実行するには、 ``Application`` を作成し、そこにコマンドを追加する必要があります。::

    #!/usr/bin/env php
    <?php
    // app/console

    use Acme\DemoBundle\Command\GreetCommand;
    use Symfony\Component\Console\Application;

    $application = new Application();
    $application->add(new GreetCommand);
    $application->run();

次のように新しく作成したコンソールコマンドを実行してみましょう。

.. code-block:: bash

    $ app/console demo:greet Fabien

結果として次の様にコマンドライン上に表示されます。

.. code-block:: text

    Hello Fabien

また ``--yell`` オプションを使用し、大文字で表示することができます。

.. code-block:: bash

    $ app/console demo:greet Fabien --yell

結果は以下の通りです。::

    HELLO FABIEN

.. _components-console-coloring:

出力に色を付ける
~~~~~~~~~~~~~~~~~~~

テキストを出力する際に、タグの付いたテキストに色を付けることができます。例えば ::

    // green text
    $output->writeln('<info>foo</info>');

    // yellow text
    $output->writeln('<comment>foo</comment>');

    // black text on a cyan background
    $output->writeln('<question>foo</question>');

    // white text on a red background
    $output->writeln('<error>foo</error>');

:class:`Symfony\\Component\\Console\\Formatter\\OutputFormatterStyle` を使用して、独自のスタイル定義をすることも可能です。::

    $style = new OutputFormatterStyle('red', 'yellow', array('bold', 'blink'));
    $output->getFormatter()->setStyle('fire', $style);
    $output->writeln('<fire>foo</fire>');

文字色及び背景色には次の色が指定できます。: ``black``, ``red``, ``green``,
``yellow``, ``blue``, ``magenta``, ``cyan``, ``white``.

オプションとしては次のものが指定できます。: ``bold``, ``underscore``, ``blink``, ``reverse``, ``conceal``.

色とオプションはタグの中でも設定できます。::

    // green text
    $output->writeln('<fg=green>foo</fg=green>');

    // black text on a cyan background
    $output->writeln('<fg=black;bg=cyan>foo</fg=black;bg=cyan>');

    // bold text on a yellow background
    $output->writeln('<bg=yellow;options=bold>foo</bg=yellow;options=bold>');

詳細表示レベル
~~~~~~~~~~~~~~

.. versionadded:: 2.3
   ``VERBOSITY_VERY_VERBOSE`` 及び ``VERBOSITY_DEBUG`` 定数は v2.3 で導入されました。

5つの詳細レベルがあります。これらは、 :class:`Symfony\\Component\\Console\\Output\\OutputInterface` で定義されています。

=======================================  ==================================
モード                                   値
=======================================  ==================================
OutputInterface::VERBOSITY_QUIET         何も出力させない
OutputInterface::VERBOSITY_NORMAL        デフォル
OutputInterface::VERBOSITY_VERBOSE       詳細情報を表示
OutputInterface::VERBOSITY_VERY_VERBOSE  必須ではない詳細情報も表示
OutputInterface::VERBOSITY_DEBUG         デバッグ情報を表示
=======================================  ==================================

出力無しにさせたいときは、 ``--quiet`` もしくは ``-q`` を指定します。
詳細度を増やしたいときは、 ``--verbose`` を ``-v`` を指定します。

.. tip::

    例外のスタックトレースを全て表示するには、 ``VERBOSITY_VERBOSE`` 以上を指定します。

特定の詳細レベルでのみの出力をするには、次のようにします。::

    if (OutputInterface::VERBOSITY_VERBOSE <= $output->getVerbosity()) {
        $output->writeln(...);
    }

.. versionadded:: 2.4
   :method:`Symfony\\Component\Console\\Output\\Output::isQuiet`,
   :method:`Symfony\\Component\Console\\Output\\Output::isVerbose`,
   :method:`Symfony\\Component\Console\\Output\\Output::isVeryVerbose`, 
   :method:`Symfony\\Component\Console\\Output\\Output::isDebug`
   は v2.4 で導入されました。

より表意的なメソッドを使用して、各詳細レベルで調べることができます。

詳細レベル::

    if ($output->isQuiet()) {
        // ...
    }

    if ($output->isVerbose()) {
        // ...
    }

    if ($output->isVeryVerbose()) {
        // ...
    }

    if ($output->isDebug()) {
        // ...
    }

quiet レベルの場合は、デフォルトの
:method:`Symfony\Component\Console\Output::write <Symfony\\Component\\Console\\Output::write>`
メソッドが、何も出力されること無く返ります。

コマンド引数を使う
------------------

コマンドの最も興味深い部分は、指定可能な引数とオプションです。
引数はスペースで区切られた文字列で、コマンドラインに続いて指定します。
これは順序があり、オプションや必須項目であるという指定ができます。
例えば、コマンドにオプションの ``last_name`` 引数、、必須項目の ``name`` 引数を追加してみます。 ::

    $this
        // ...
        ->addArgument(
            'name',
            InputArgument::REQUIRED,
            'Who do you want to greet?'
        )
        ->addArgument(
            'last_name',
            InputArgument::OPTIONAL,
            'Your last name?'
        );

これで、次のようにコマンドの ``last_name`` 引数を受け取ることができるようになりました。 ::

    if ($lastName = $input->getArgument('last_name')) {
        $text .= ' '.$lastName;
    }

結果、コマンドは、次のように使用できるようになりました。

.. code-block:: bash

    $ app/console demo:greet Fabien
    $ app/console demo:greet Fabien Potencier

引数としてリストを受けることも出来ます(友達全員に挨拶したいこともあるでしょう)。
最後の引数に、指定することでこれを実現できます。::

    $this
        // ...
        ->addArgument(
            'names',
            InputArgument::IS_ARRAY,
            'Who do you want to greet (separate multiple names with a space)?'
        );

好きなだけ名前を指定すればいよいです。

.. code-block:: bash

    $ app/console demo:greet Fabien Ryan Bernhard

引数 ``names`` へは、配列としてアクセス出来ます::

    if ($names = $input->getArgument('names')) {
        $text .= ' '.implode(', ', $names);
    }

3種類の引数があります。

===========================  ========================================================
モード                       値
===========================  ========================================================
InputArgument::REQUIRED      必須。
InputArgument::OPTIONAL      任意。省略可。
InputArgument::IS_ARRAY      可変長の引数。最後の引数として使用されなければならない。
===========================  ========================================================

``IS_ARRAY`` は ``REQUIRED`` や ``OPTIONAL`` と同時に指定できます。::

    $this
        // ...
        ->addArgument(
            'names',
            InputArgument::IS_ARRAY | InputArgument::REQUIRED,
            'Who do you want to greet (separate multiple names with a space)?'
        );

コマンドのオプションの使用方法
------------------------------

引数とは違い、オプションは指定する順番は関係がありません。
そして、 ``--yell`` のようにハイフンを２つ使用し、指定します。
実際は、ショートカットとして ``-y`` のようにハイフン1つ使用し、
1文字で指定することもできます。
オプションは *必ず* 指定しなくても問題ありません。
また、 ``dir=src`` のような値も有効ですし、 ``yell`` のように単純に値無しの真偽値としても有効です。

.. tip::

    オプションが、値を受けるかどうかを *オプションに* することもできます。
    (つまり、``--yell`` や ``yell=loud`` が動くように。)
    また、配列して値を受け取るオプションも設定できます。

例として、コマンドに新しいオプションを追加してみましょう。
このオプションは、表示するメッセージの回数を指定することにします。 ::

    $this
        // ...
        ->addOption(
            'iterations',
            null,
            InputOption::VALUE_REQUIRED,
            'How many times should the message be printed?',
            1
        );

次に、複数回このメッセージを表示するように、
このコマンド内でオプションである ``iterations`` を使用します。


.. code-block:: php

    for ($i = 0; $i < $input->getOption('iterations'); $i++) {
        $output->writeln($text);
    }

これでタスクを実行すれば、 ``--iterations`` のフラグをオプションとして指定できるようになりました。

.. code-block:: bash

    $ app/console demo:greet Fabien
    $ app/console demo:greet Fabien --iterations=5

最初の例では、 ``iterations`` を渡していないので、一度だけ表示します。
これは、 ``addOption`` メソッドの最後の引数でデフォルト値に 1 を指定しているからです。
そして2つ目の例では、5回表示します。

オプションには、順番は関係ないので、次の例のどちらも同じように動作します。

.. code-block:: bash

    $ app/console demo:greet Fabien --iterations=5 --yell
    $ app/console demo:greet Fabien --yell --iterations=5

オプションには4種類あります。

===========================  =====================================================================================
Option                       Value
===========================  =====================================================================================
InputOption::VALUE_IS_ARRAY  複数の値の指定可 (例 ``--dir=/foo --dir=/bar``)
InputOption::VALUE_NONE      値を受け付けない (例 ``--yell``)
InputOption::VALUE_REQUIRED  値が必須 (例 ``--iterations=5``) オプション自体は任意
InputOption::VALUE_OPTIONAL  値は任意 (例 ``yell`` or ``yell=loud``)
===========================  =====================================================================================

``VALUE_IS_ARRAY`` は ``VALUE_REQUIRED`` や ``VALUE_OPTIONAL`` と同時に指定することが出来ます。

.. code-block:: php

    $this
        // ...
        ->addOption(
            'iterations',
            null,
            InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY,
            'How many times should the message be printed?',
            1
        );

コンソールヘルパー
------------------

Console コンポーネントは、「ヘルパー」セットが入っています。
色々な作業の時に便利な小さめのツール群です。

* :doc:`/components/console/helpers/dialoghelper`: ユーザにインタラクティブに入力を求める
* :doc:`/components/console/helpers/formatterhelper`: 出力の色付けを変更する
* :doc:`/components/console/helpers/progresshelper`: プログレスバー
* :doc:`/components/console/helpers/tablehelper`: 表形式のデータをテーブルで表示

コマンドのテスト
----------------

Symfony2 はコマンドを容易にテストできるようになるツールをいくつか用意しています。
最も便利なものは、 :class:`Symfony\\Component\\Console\\Tester\\CommandTester` クラスです。
このクラスは、実際のコンソールが無くてもテストするために、特別な入力と出力のクラスを使用します。 ::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

:method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay` メソッドは、
コンソールからのコマンド実行で、表示されるはずの結果を返します。

コマンドに対する引数やオプションの指定は、
:method:`Symfony\\Component\\Console\\Tester\\CommandTester::execute` メソッドに対して、
配列を送ることで行います。::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        // ...

        public function testNameIsOutput()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
                array('command' => $command->getName(), 'name' => 'Fabien')
            );

            $this->assertRegExp('/Fabien/', $commandTester->getDisplay());
        }
    }

.. tip::

    :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester` クラスを使用すれば、
    全てのコンソールアプリケーションのテストもできます。

すでにあるコマンドの呼び出し
----------------------------

あるコマンドを実行する前に、他のコマンドを既に実行し終わっているべきだ、という順番の管理が必要なときもあるでしょう。
実行の順番をユーザに覚えてもらうのではなく、自分身で直接管理することができます。
たくさんのコマンドをまとめて実行する "meta" コマンドを作成する際に便利です。 
"meta" コマンドとは、例えば本番サーバのプロジェクトのコードを変更した際に
、実行すべき全てのコードをまとめたものです。
キャッシュのクリア、 Doctrine2 のプロクシの生成、 Assetic アセットのダンプなどなど。

コマンドから他のコマンドを呼ぶのは簡単で、次のようできます。 ::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $command = $this->getApplication()->find('demo:greet');

        $arguments = array(
            'command' => 'demo:greet',
            'name'    => 'Fabien',
            '--yell'  => true,
        );

        $input = new ArrayInput($arguments);
        $returnCode = $command->run($input, $output);

        // ...
    }

まず、 :method:`Symfony\\Component\\Console\\Application::find` メソッドにコマンド名を渡し、実行したいコマンドを探します。

そして、指定したい引数とオプションを渡し
:class:`Symfony\\Component\\Console\\Input\\ArrayInput` クラスを新しく作成します。

最後に、 ``run()`` メソッドを呼んで、実際にコマンドを実行し、そのコマンドの戻り値を戻します。
コマンドの ``execute()`` メソッドが返す値です。

.. note::

    ほとんどの場合、コマンドライン上で実行されないコードからコマンドを呼び出すのは、
    次の理由から良いアイデアではありません。
    まず、コマンドの出力は、コンソールのために最適化されています。
    しかし、より大事なこととして、コマンドをコントローラのように考えることができます。
    コントローラは、モデルを使用し処理を行い、ユーザにフィードバックを表示します。
    ウェブからコマンドを呼ぶのではなく、コードをリファクタリングして、ロジックを新しいクラスに移すべきです。

さらに!
-------

* :doc:`/components/console/usage`
* :doc:`/components/console/single_command_tool`

.. _Packagist: https://packagist.org/packages/symfony/console
.. _ANSICON: https://github.com/adoxa/ansicon/downloads

.. 2012/01/21 77web 068bfa62d50062ec15db1942328d34f6c10a65b5
.. 2013/11/23 gilbite 55a2de49dce07919f5e79f0d1aaded50e37fe934  components/console.rst => components/console/introduction.rst and update
