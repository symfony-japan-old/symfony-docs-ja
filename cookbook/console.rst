.. index::
    single: Console; CLI


コンソール/コマンドラインツールとしてのコマンドの作成方法
===========================================

Symfony2 はコマンドラインツールとしてのコマンドを作成することのできるコンソールコンポーネントが付いてきます。コンソールコマンドは、cronジョブやインポートなどのバッチジョブなどの自動更新タスクに使用されます。

ベーシックなコマンドの作成
------------------------

Symfony2 において、コンソールコマンドを自動的に使用可能にするには、 ``Command`` ディレクトリをバンドル内に作成し、その中に提供したいコマンドを実装した ``Command.php`` という接尾辞を追加した PHP ファイルを作成してください。例えば、 Symfony Standard Edition に含まれている ``AcmeDemoBundle`` を拡張し、コマンドラインから挨拶をしようとするならば、次のように ``GreetCommand.php`` ファイルを作成してください。
::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Greet someone')
                ->addArgument('name', InputArgument::OPTIONAL, 'Who do you want to greet?')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'If set, the task will yell in uppercase letters')
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

次のように新しく作成したコンソールコマンドを実行してみましょう。

.. code-block:: bash

    app/console demo:greet Fabien

結果として次のコマンドラインが表示されます。

.. code-block:: text

    Hello Fabien

また ``--yell`` オプションを使用し、大文字で表示することができます。

.. code-block:: bash

    app/console demo:greet Fabien --yell

結果は以下の通りです。::

    HELLO FABIEN

出力に色を付ける
~~~~~~~~~~~~~~~~~~~

テキストを出力する際に、タグの付いたテキストに色を付けることができます。例えば
::

    // green text
    $output->writeln('<info>foo</info>');

    // yellow text
    $output->writeln('<comment>foo</comment>');

    // black text on a cyan background
    $output->writeln('<question>foo</question>');

    // white text on a red background
    $output->writeln('<error>foo</error>');

コマンドラインの引数の使用
-----------------------

コマンドの最も興味深い部分は、指定可能な引数とオプションです。引数はスペースで区切られた文字列で、コマンドラインに続いて指定します。これは順序があり、オプションや必須項目であるという指定ができます。例えば、コマンドにオプションの ``last_name`` 引数、、必須項目の ``name`` 引数を追加してみます。
::

    $this
        // ...
        ->addArgument('name', InputArgument::REQUIRED, 'Who do you want to greet?')
        ->addArgument('last_name', InputArgument::OPTIONAL, 'Your last name?')
        // ...

これで、次のようにコマンドの ``last_name`` 引数を受け取ることができるようになりました。
::

    if ($lastName = $input->getArgument('last_name')) {
        $text .= ' '.$lastName;
    }

結果、コマンドは、次のように使用できるようになりました。

.. code-block:: bash

    app/console demo:greet Fabien
    app/console demo:greet Fabien Potencier

コマンドのオプションの使用方法
---------------------

引数とは違い、オプションは指定する順番は関係がありません。そして、 ``--yell`` のようにハイフンを２つ使用し、指定します。実際は、ショートカットとして ``-y`` のようにハイフン１つ使用し、１文字で指定することもできます。オプションは *必ず* 指定しなくても問題ありません。また、 ``dir=src`` のような値も有効ですし、 ``yell`` のように単純に値無しの真偽値としても有効です。

.. tip::

    さらに、オプションに ``--yell`` や ``yell=loud`` のように、どちらでも使用できるような値を受け取らせることも *可能* です。

例として、コマンドに新しいオプションを追加してみましょう。このオプションは、表示するメッセージの回数を指定することにします。
::

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED, 'How many times should the message be printed?', 1)

次に、複数回このメッセージを表示するように、このコマンド内でオプションである ``iterations`` を使用します。

.. code-block:: php

    for ($i = 0; $i < $input->getOption('iterations'); $i++) {
        $output->writeln($text);
    }

これでタスクを実行すれば、 ``--iterations`` のフラグをオプションとして指定できるようになりました。

.. code-block:: bash

    app/console demo:greet Fabien

    app/console demo:greet Fabien --iterations=5

最初の例では、 ``iterations`` を渡していないので、一度だけ表示します。これは、 ``addOption`` メソッドの最後の引数でデフォルト値に 1 を指定しているからです。そして２つ目の例では、５回表示します。

オプションには、順番は関係ないので、次の例のどちらも同じように動作します。

.. code-block:: bash

    app/console demo:greet Fabien --iterations=5 --yell
    app/console demo:greet Fabien --yell --iterations=5

４つのオプションが使用できます。:

===========================  =====================================================
Option                       Value
===========================  =====================================================
InputOption::VALUE_IS_ARRAY  このオプションは複数の値を受け取ります
InputOption::VALUE_NONE      このオプションへの入力を受け取りません (e.g. ``--yell``)
InputOption::VALUE_REQUIRED  値は必須です (e.g. ``iterations=5``)
InputOption::VALUE_OPTIONAL  値はオプショナルです
===========================  =====================================================

次のように VALUE_REQUIRED と VALUE_OPTIONAL を組み合わせた VALUE_IS_ARRAY も可能です。

.. code-block:: php

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY, 'How many times should the message be printed?', 1)

ユーザに情報を尋ねる
-------------------------------

コマンドを作成する際に、ユーザに質問を尋ねて情報を集めることもできます。例えば、あるアクションに対して実行前にユーザに確認させるようにしたいとしましょう。次のようにしてください。
::

    $dialog = $this->getHelperSet()->get('dialog');
    if (!$dialog->askConfirmation($output, '<question>Continue with this action?</question>', false)) {
        return;
    }

このケースでは、ユーザに "Continue with the action" と尋ねています。そして、ユーザが ``y`` を返さなければこのタスクは実行しないようにします。 ``askConfirmation`` の３つ目の引数は、ユーザが何も入力しなかった際のデフォルト値です。

また、単なる yes/no の答え以外にも質問を尋ねることができます。例えば、何かの名前を知りたいとしましょう。その際には、次のようにします。
::

    $dialog = $this->getHelperSet()->get('dialog');
    $name = $dialog->ask($output, 'Please enter the name of the widget', 'foo');

コマンドのテスト
----------------

Symfony2 はコマンドを容易にテストできるようになるツールをいくつか用意しています。最も便利なものは、 :class:`Symfony\\Component\\Console\\Tester\\CommandTester` クラスです。このクラスは、実際のコンソール無しでテストができるように、特別な入力と出力のクラスを使用します。
::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand.php;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // mock the Kernel or create one depending on your needs
            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

:method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay` メソッドは、コンソールからのコマンド実行で、表示されるはずの結果を返します。

.. tip::

    :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester` クラスを使用すれば、全てのコンソールアプリケーションのテストもできます。

サービスコンテナからサービスを取得する
-------------------------------------------

コマンドのベースクラスに :class:`Symfony\Component\Console\Command\Command` ではなく、 :class:`Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand` を使用すれば、サービスコンテナへのアクセスもできるようになります。 つまり、設定された全てのサービスにアクセスができるのです。例えば次のように、簡単にタスクを拡張して、翻訳可能にもできます。
::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $translator = $this->getContainer()->get('translator');
        if ($name) {
            $output->writeln($translator->trans('Hello %name%!', array('%name%' => $name)));
        } else {
            $output->writeln($translator->trans('Hello!'));
        }
    }

すでにあるコマンドの呼び出し
---------------------------

あるコマンドを実行する前に、他のコマンドを既に実行し終わっていないと、いけないという順番の管理が必要なときもあるでしょう。実行の順番をユーザに覚えてもらうのではなく、あなた自身で直接管理することができます。たくさんのコマンドをまとめて実行する "meta" コマンドを作成する際に便利です。 "meta" コマンドとは、例えば本番サーバのプロジェクトのコードを変更した際に、実行すべき全てのコードをまとめたものです。キャッシュのクリア、 Doctrine2 のプロクシの生成、 Assetic アセットのダンプなどなど。

コマンドから他のコマンドを呼ぶのは簡単で、次のようできます。
::

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

まず、 :method:`Symfony\\Component\\Console\\Command\\Command::find` メソッドにコマンド名を渡し、実行したいコマンドを探します。

そして、指定したい引数とオプションを渡し :class:`Symfony\\Component\\Console\\Input\\ArrayInput` クラスを新しく作成します。

最後に、 ``run()`` メソッドを呼んで、実際にコマンドを実行し、そのコマンドの返り値を返します。全てうまく行けば ``0`` が返ってきますし、何か問題があれば、他の整数値が返ってきます。

.. note::

    ほとんどの場合、コマンドライン上で実行されないコードからコマンドを呼び出すのは、次の理由から良いアイデアではありません。まず、コマンドの出力は、コンソールのために最適化されています。しかし、より大事なこととして、コマンドをコントローラのように考えることができます。コントローラは、モデルを使用し処理を行い、ユーザにフィードバックを表示します。ウェブからコマンドを呼ぶのではなく、コードをリファクタリングして、ロジックを新しいクラスに移すべきです。

.. 2011/11/28 ganchiku e39f5a8b06c0f1ed0634829ac9fbc02a7ac5523d

