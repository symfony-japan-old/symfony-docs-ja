.. index::
   single: Service Container; Tags

サービスにタグを使用する方法
==================================

Symfony2 のコアサービスには、どのサービスが、ロードされるべきか、イベントに通知されるべきか、何か特別な方法で処理すべきか、といったことを決めるためのタグに依存しているものがあります。例えば、 Twig は ``twig.extension`` というタグを使用し、追加のエクステンションをロードしています。

これらのタグは、あなたの開発するバンドルにも使用することができます。例えば、あなたのサービスが何かのコレクションを処理したり、"チェーン" を実装して、成功するまで代替戦略を試みたりするときなどです。この記事では、 ``\Swift_Transport`` を実装するクラスのコレクションである "transport chain" の例を使用します。このチェーンを使用して、成功するまで Swiftmailer がいくつかのトランスポートの使用を試みます。この内容は、主に DI に関して着目しています。

まず始めに、 ``TransportChain`` クラスを定義します
::

    namespace Acme\MailerBundle;
    
    class TransportChain
    {
        private $transports;
    
        public function __construct()
        {
            $this->transports = array();
        }
    
        public function addTransport(\Swift_Transport  $transport)
        {
            $this->transports[] = $transport;
        }
    }

次に、チェーンをサービスとして定義します
:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/MailerBundle/Resources/config/services.yml
        parameters:
            acme_mailer.transport_chain.class: Acme\MailerBundle\TransportChain
        
        services:
            acme_mailer.transport_chain:
                class: %acme_mailer.transport_chain.class%

    .. code-block:: xml

        <!-- src/Acme/MailerBundle/Resources/config/services.xml -->

        <parameters>
            <parameter key="acme_mailer.transport_chain.class">Acme\MailerBundle\TransportChain</parameter>
        </parameters>
    
        <services>
            <service id="acme_mailer.transport_chain" class="%acme_mailer.transport_chain.class%" />
        </services>
        
    .. code-block:: php
    
        // src/Acme/MailerBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        
        $container->setParameter('acme_mailer.transport_chain.class', 'Acme\MailerBundle\TransportChain');
        
        $container->setDefinition('acme_mailer.transport_chain', new Definition('%acme_mailer.transport_chain.class%'));

サービスをカスタムタグで定義します
---------------------------------

これで複数の ``\Swift_Transport`` クラスを初期化させる必要になりましたので、 ``addTransport()`` メソッドを使用し自動的にチェーンへ追加していきます。例として、次のトランスポートをサービスとして追加します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/MailerBundle/Resources/config/services.yml
        services:
            acme_mailer.transport.smtp:
                class: \Swift_SmtpTransport
                arguments:
                    - %mailer_host%
                tags:
                    -  { name: acme_mailer.transport }
            acme_mailer.transport.sendmail:
                class: \Swift_SendmailTransport
                tags:
                    -  { name: acme_mailer.transport }
    
    .. code-block:: xml

        <!-- src/Acme/MailerBundle/Resources/config/services.xml -->
        <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
            <argument>%mailer_host%</argument>
            <tag name="acme_mailer.transport" />
        </service>
    
        <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
            <tag name="acme_mailer.transport" />
        </service>
        
    .. code-block:: php
    
        // src/Acme/MailerBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        
        $definitionSmtp = new Definition('\Swift_SmtpTransport', array('%mailer_host%'));
        $definitionSmtp->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.smtp', $definitionSmtp);
        
        $definitionSendmail = new Definition('\Swift_SendmailTransport');
        $definitionSendmail->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.sendmail', $definitionSendmail);

"acme_mailer.transport" として名付けられたタグを通知します。バンドルにこれらのトランスポートを認識させ、自身でこれらのトランスポートをチェーンに追加させるようにしましょう。そのために、 ``AcmeMailerBundle`` クラスに ``build()`` メソッドを追加してください。
::

    namespace Acme\MailerBundle;
    
    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;
    
    use Acme\MailerBundle\DependencyInjection\Compiler\TransportCompilerPass;
    
    class AcmeMailerBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);
    
            $container->addCompilerPass(new TransportCompilerPass());
        }
    }

``CompilerPass`` を作成する
-------------------------

まだ作成していない ``TransportCompilerPass`` クラスへのリファレンスに気づくでしょう。このクラスは、 ``addTransport()`` メソッドを呼び ``acme_mailer.transport`` でタグ付けされた全てのサービスを ``TransportChain`` クラスに追加させます。 ``TransportCompilerPass`` は次のようになります。
::

    namespace Acme\MailerBundle\DependencyInjection\Compiler;
    
    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;
    
    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            if (false === $container->hasDefinition('acme_mailer.transport_chain')) {
                return;
            }
    
            $definition = $container->getDefinition('acme_mailer.transport_chain');
    
            foreach ($container->findTaggedServiceIds('acme_mailer.transport') as $id => $attributes) {
                $definition->addMethodCall('addTransport', array(new Reference($id)));
            }
        }
    }

``process()`` メソッドは ``acme_mailer.transport_chain`` サービスの存在をチェックし、 ``acme_mailer.transport`` にタグ付けされた全てのサービスを調べます。そして、 ``addTransport()`` を呼び、全ての "acme_mailer.transport" サービスを ``acme_mailer.transport_chain`` の定義に加えます。これらの呼び出しの最初の引数は、メーラートランスポートサービスそのものになります。

.. note::

    慣習として、タグ名は、バンドルの名前(小文字で区切りはアンダースコア)にドットが次に来て、 "実際の" 名前をつなげます。つまり、 AcmeMailerBundle の "transport" タグであれば ``acme_mailer.transport`` になります。

コンパイルされたサービスの定義
-------------------------------

コンパイルを通すと、コンパイルされたサービスコンテナに次の行を自動的に生成することになります。 "dev" 環境で動かしているのであれば、 ``/cache/dev/appDevDebugProjectContainer.php`` を開き、 ``getTransportChainService()`` メソッドを調べてみてください。次のようになっているはずです。
::

    protected function getAcmeMailer_TransportChainService()
    {
        $this->services['acme_mailer.transport_chain'] = $instance = new \Acme\MailerBundle\TransportChain();

        $instance->addTransport($this->get('acme_mailer.transport.smtp'));
        $instance->addTransport($this->get('acme_mailer.transport.sendmail'));

        return $instance;
    }

.. 2011/12/27 ganchiku 3512e0524ed0b96c3df3277cfa1f39d6c4591e02

