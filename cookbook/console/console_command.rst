.. index::
   single: Console; Create commands

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/10/12

コンソールコマンドの作り方
==========================

Console コンポーネントの解説 (:doc:`/components/console/introduction`) では、コンソールコマンドの基本的な作り方のみを解説しました。
このページでは、Symfony2 フレームワーク上でコンソールコマンドを作る場合に特化した内容を解説します。

自動的に登録されるコマンド
--------------------------

作成したコンソールコマンドを Symfony2 で自動的に利用可能にするには、
バンドル内に ``Command`` ディレクトリを作成し、提供したいコマンドの PHP ファイルを作成します。ファイルのサフィックスは ``Command.php`` とします。
たとえば、Symfony Standard Edition に付属する ``AcmeDemoBundle`` にコマンドラインからあいさつをする機能を追加する場合、バンドルの ``Command`` ディレクトリに ``GreetCommand.php`` ファイルを作成し、次の内容で保存してください。

.. code-block:: php

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
                ->setDescription('誰かにあいさつをする')
                ->addArgument('name', InputArgument::OPTIONAL, 'あいさつをする相手の名前')
                ->addOption('yell', null, InputOption::VALUE_NONE, '指定した場合、大文字であいさつを返します')
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

これで、自動的にコンソールからコマンドを実行できるようになります。次のように実行します。

.. code-block:: bash

    $ app/console demo:greet Fabien

サービスコンテナからサービスを取得する
--------------------------------------

作成するコマンドの基底クラスとして :class:`Symfony\\Component\\Console\\Command\\Command` のかわりに :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand` を使っているので、サービスコンテナに手軽にアクセスし、サービスを利用できます。
たとえば、\ ``translator`` サービスを使ってコマンドのメッセージを翻訳に対応させるといったことが可能です。

.. code-block:: php

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

コマンドのテスト
----------------

コマンドを完全に機能するフレームワーク上でテストしたい場合、\ :class:`Symfony\\Component\\Console\\Application <Symfony\\Component\\Console\\Application>` のかわりに :class:`Symfony\\Bundle\\FrameworkBundle\\Console\\Application <Symfony\\Bundle\\FrameworkBundle\\Console\\Application>` を使います。

.. code-block:: php

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // mock the Kernel or create one depending on your needs
            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
               array(
                  'command' => $command->getName(), // Symfony 2.3までは必要
                  'name'    => 'Fabien',
                  '--yell'  => true,
               )
            );

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

.. note::

    上のケースでは、\ ``name`` パラメーターと ``--yell`` オプションはコマンドの実行に必須ではありません。
    コマンド呼び出し時のパラメーターとオプションのカスタマイズ方法を説明する目的で示しています。

.. note::

    Symfony 2.3 までは、\ ``CommandTest`` の ``execute()`` に渡す連想配列には、コマンド自身の名前を ``command`` キーで指定する必要があります。
    Symfony 2.4 以降では、コマンド自身の名前が自動登録されるようになりました。
    `[Console] pass command name automatically if required by the application`_

コンソールコマンドのテストで完全にセットアップされたサービスコンテナを利用したい場合は、\ :class:`Symfony\\Bundle\\FrameworkBundle\\Test\\WebTestCase` を継承してテストクラスを作成してください。

.. code-block:: php

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends WebTestCase
    {
        public function testExecute()
        {
            $kernel = $this->createKernel();
            $kernel->boot();

            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
               array(
                  'command' => $command->getName(), // Symfony 2.3までは必要
                  'name'    => 'Fabien',
                  '--yell'  => true,
               )
            );

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

.. _`[Console] pass command name automatically if required by the application`: https://github.com/symfony/symfony/pull/8626

.. 2013/10/12 hidenorigoto 16e557750186fe690a8af61a1cb47742d8da2d05

