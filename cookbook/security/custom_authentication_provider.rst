.. index::
   single: Security; Custom Authentication Provider

カスタム認証プロバイダの作成方法
==============================================

ドキュメントの :doc:`/book/security` の章を読めば、 Symfony2 では、セキュリティの実装における認証(authentication)と承認(authorization)の違いを理解することができます。この章では、認証プロセスに関するコアのクラス群を見ていきます。そして、カスタム認証プロバイダの実装方法を学びます。認証と承認は、異なるコンセプトのため、今回の拡張は、ユーザプロバイダ自体は特定化しておらず、メモリ上、データベース、他の格納場所などのユーザプロバイダと機能することができます。

WSSE を使用する
---------

この章では、 WSSE 認証を使用したカスタム認証プロバイダの作成方法を実演します。 WSSE のセキュリティプロトコルは、次のセキュリティに関する利点があります。

1. ユーザ名 / パスワード暗号化
2. リプレイアタックに対する安全な保護
3. ウェブサーバの設定が必要ないこと

WSSE は、 SOAP や REST を使用しているウェブサービスのセキュリティにおいてとても便利です。

`WSSE`_ に関するドキュメントはとても豊富ですが、この章では、セキュリティプロトコルに自体よりも、 Symfony2 のアプリケーションにカスタムプロトコルを追加するマナーについて焦点を充てています。 WSSE の基本は、リクエストヘッダが暗号化された証明書をチェックして、タイムスタンプと `nonce`_ を使用して検証し、パスワードダイジェストを使用してリクエストしてきたユーザを認証します。

.. note::

    WSSE は、認証キーのバリデーションもサポートしています。ウェブサービスにはとても便利ですが、この章では扱いません。

トークン
---------

Symfony2 のセキュリティコンテキストのトークンの役割は、重要です。トークンはリクエストのユーザ認証データを表しています。リクエストが一度認証されれば、トークンはユーザデータを保持し、セキュリティコンテキストに渡します。まず、トークンクラスを作成します。このクラスは、関係する全ての情報を認証プロバイダに渡せるようにしています。

.. code-block:: php

    // src/Acme/DemoBundle/Security/Authentication/Token/WsseUserToken.php
    namespace Acme\DemoBundle\Security\Authentication\Token;

    use Symfony\Component\Security\Core\Authentication\Token\AbstractToken;

    class WsseUserToken extends AbstractToken
    {
        public $created;
        public $digest;
        public $nonce;

        public function getCredentials()
        {
            return '';
        }
    }

.. note::

    ``WsseUserToken`` クラスはベーストークンクラス機能を提供するセキュリティコンポーネントである :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AbstractToken` を拡張しています。トークンとして使用する全てのクラスは、 :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface` を実装してください。

リスナ
------------

次に、セキュリティコンテキストをリッスンするリスナが必要です。リスナは、ファイアーウォールへのリクエストや認証プロバイダの呼び出しを受け止める役割を担います。リスナは、 :class:`Symfony\\Component\\Security\\Http\\Firewall\\ListenerInterface` を実装したクラスのインスタンスである必要があります。セキュリティリスナは、 :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent` イベントをリッスンし、成功時に認証されたトークンをセキュリティコンテキストにセットする必要があります。

.. code-block:: php

    // src/Acme/DemoBundle/Security/Firewall/WsseListener.php
    namespace Acme\DemoBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\Security\Http\Firewall\ListenerInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    use Symfony\Component\Security\Core\Authentication\AuthenticationManagerInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Acme\DemoBundle\Security\Authentication\Token\WsseUserToken;

    class WsseListener implements ListenerInterface
    {
        protected $securityContext;
        protected $authenticationManager;

        public function __construct(SecurityContextInterface $securityContext, AuthenticationManagerInterface $authenticationManager)
        {
            $this->securityContext = $securityContext;
            $this->authenticationManager = $authenticationManager;
        }

        public function handle(GetResponseEvent $event)
        {
            $request = $event->getRequest();

            if (!$request->headers->has('x-wsse')) {
                return;
            }

            $wsseRegex = '/UsernameToken Username="([^"]+)", PasswordDigest="([^"]+)", Nonce="([^"]+)", Created="([^"]+)"/';

            if (preg_match($wsseRegex, $request->headers->get('x-wsse'), $matches)) {
                $token = new WsseUserToken();
                $token->setUser($matches[1]);

                $token->digest   = $matches[2];
                $token->nonce    = $matches[3];
                $token->created  = $matches[4];

                try {
                    $returnValue = $this->authenticationManager->authenticate($token);

                    if ($returnValue instanceof TokenInterface) {
                        return $this->securityContext->setToken($returnValue);
                    } else if ($returnValue instanceof Response) {
                        return $event->setResponse($returnValue);
                    }
                } catch (AuthenticationException $e) {
                    // you might log something here
                }
            }

            $response = new Response();
            $response->setStatusCode(403);
            $event->setResponse($response);
        }
    }

このリスナは、 `X-WSSE` ヘッダがあることを想定して、リクエストをチェックします。そして、返ってきた値と想定している WSSE 情報の照合をします。そして、その情報を使用してトークンを作成し、認証マネージャにトークンを渡します。その情報が適切でなければ、認証マネージャは、 :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException` を投げて 403 のレスポンスが返されます。

.. note::

    上のコードでは使用していませんが、 :class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener` はとても便利なベースクラスで、セキュリティ拡張でよく使われる機能を用意しています。このクラスは、セッションにトークンを維持したり、成功ハンドラ / 失敗ハンドラやログインフォームの URL を提供することができます。 WSSE は認証セッションの維持もログインフォームも必要ないので、ここでは使用しませんでした。

認証プロバイダ
---------------------------

認証プロバイダは、 ``WsseUserToken`` の検証を行います。このプロバイダは、 ``Created`` ヘッダ値が５分間有効であること、 ``Nonce`` ヘッダ値が５分間ユニークであるあること、そして、 ``PasswordDigest`` ヘッダ値がユーザのパスワードに一致していることを検証します。

.. code-block:: php

    // src/Acme/DemoBundle/Security/Authentication/Provider/WsseProvider.php
    namespace Acme\DemoBundle\Security\Authentication\Provider;

    use Symfony\Component\Security\Core\Authentication\Provider\AuthenticationProviderInterface;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\Exception\NonceExpiredException;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Acme\DemoBundle\Security\Authentication\Token\WsseUserToken;

    class WsseProvider implements AuthenticationProviderInterface
    {
        private $userProvider;
        private $cacheDir;

        public function __construct(UserProviderInterface $userProvider, $cacheDir)
        {
            $this->userProvider = $userProvider;
            $this->cacheDir     = $cacheDir;
        }

        public function authenticate(TokenInterface $token)
        {
            $user = $this->userProvider->loadUserByUsername($token->getUsername());

            if ($user && $this->validateDigest($token->digest, $token->nonce, $token->created, $user->getPassword())) {            
                $authenticatedToken = new WsseUserToken($user->getRoles());
                $authenticatedToken->setUser($user);

                return $authenticatedToken;
            }

            throw new AuthenticationException('The WSSE authentication failed.');
        }

        protected function validateDigest($digest, $nonce, $created, $secret)
        {
            // Expire timestamp after 5 minutes
            if (time() - strtotime($created) > 300) {
                return false;
            }

            // Validate nonce is unique within 5 minutes
            if (file_exists($this->cacheDir.'/'.$nonce) && file_get_contents($this->cacheDir.'/'.$nonce) + 300 >= time()) {
                throw new NonceExpiredException('Previously used nonce detected');
            }
            file_put_contents($this->cacheDir.'/'.$nonce, time());

            // Validate Secret
            $expected = base64_encode(sha1(base64_decode($nonce).$created.$secret, true));

            return $digest === $expected;
        }

        public function supports(TokenInterface $token)
        {
            return $token instanceof WsseUserToken;
        }
    }

.. note::

    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface` インタフェースは、 ``authenticate`` メソッドと、与えられたトークン ``supports`` メソッドを必要とします。 ``authenticate`` メソッドでは、 ユーザのトークンを渡し、 ``supports`` メソッドでは、認証マネージャにこのプロバイダに使用するか否かを指定します。複数のプロバイダを使用している際には、認証マネージャは、リスト内の次のプロバイダに移動します。

ファクトリ
-----------

これまで、カスタムトークン、カスタムリスナ、カスタムプロバイダーを作成しました。次はこれらを全て繋げる必要があります。セキュリティコンフィギュレーションでプロバイダを使用可能にするには、 ``factory`` を使うことです。ファクトリは、プロバイダの名前と使用可能なコンフィギュレーションオプション全てを知らせて、セキュリティコンポーネントにフックさせる場所です。まず、 :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\SecurityFactoryInterface` を実装するクラスを作成する必要があります。

.. code-block:: php

    // src/Acme/DemoBundle/DependencyInjection/Security/Factory/WsseFactory.php
    namespace Acme\DemoBundle\DependencyInjection\Security\Factory;

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;
    use Symfony\Component\DependencyInjection\DefinitionDecorator;
    use Symfony\Component\Config\Definition\Builder\NodeDefinition;
    use Symfony\Bundle\SecurityBundle\DependencyInjection\Security\Factory\SecurityFactoryInterface;

    class WsseFactory implements SecurityFactoryInterface
    {
        public function create(ContainerBuilder $container, $id, $config, $userProvider, $defaultEntryPoint)
        {
            $providerId = 'security.authentication.provider.wsse.'.$id;
            $container
                ->setDefinition($providerId, new DefinitionDecorator('wsse.security.authentication.provider'))
                ->replaceArgument(0, new Reference($userProvider))
            ;

            $listenerId = 'security.authentication.listener.wsse.'.$id;
            $listener = $container->setDefinition($listenerId, new DefinitionDecorator('wsse.security.authentication.listener'));

            return array($providerId, $listenerId, $defaultEntryPoint);
        }

        public function getPosition()
        {
            return 'pre_auth';
        }

        public function getKey()
        {
            return 'wsse';
        }

        public function addConfiguration(NodeDefinition $node)
        {}
    }

:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\SecurityFactoryInterface` インタフェースは次のメソッドを必要とします。

* ``create`` メソッドは、適切なセキュリティコンテキストのため、リスナと認証プロバイダを DI コンテナに追加します。

* ``getPosition`` メソッドは、 ``pre_auth``, ``form``, ``http``, ``remembr_me`` のタイプのどれかとなり、プロバイダが呼ばれる位置を定義します。

* ``getKey`` メソッドは、プロバイダを参照するリファレンスで使用されるコンフィギュレーションキーを定義します。

* ``addConfiguration`` メソッドは、セキュリティコンフィギュレーションのキーの真下に以下されるコンフィギュレーションオプションの定義に使われます。コンフィギュレーションオプションの設定は、この章の後の方に説明があります。

.. note::

    この例では使われていませんが、 :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory` はとても便利なベースクラスで、セキュリティファクトリでよく使われる機能を用意しています。特に異なるタイプの認証プロバイダを定義する際に便利です。

これで、ファクトリクラスを作成したので、 ``wsse`` キーは、セキュリティコンフィギュレーション内のファイアーウォールとして使用できます。

.. note::

    なぜ DI コンテナにリスナとプロバイダを追加する特別なファクトリクラスを必要とするのか、疑問に持つかもしれません。理由は、アプリケーションの複数の箇所をセキュアにするために、ファイアーウォールを複数回使用することができるからです。そのため、ファイアーウォールが使われる度に、 DI コンテナ内で新しいサービスが作成されます。ファクトリは、これらの新しいサービスを作成者なのです。

コンフィギュレーション
-------------

アクションの認証プロバイダを見て行きましょう。そのために、やらなければならないことがあります。まず、上記のサービスを DI コンテナに追加します。上記のファクトリクラスは、サービスの id のリファレンスを作成します。 ``wsse.security.authentication.provider`` と ``wsse.security.authentication.listener`` です。これらのサービスを定義していきましょう。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
          wsse.security.authentication.provider:
            class:  Acme\DemoBundle\Security\Authentication\Provider\WsseProvider
            arguments: ['', %kernel.cache_dir%/security/nonces]

          wsse.security.authentication.listener:
            class:  Acme\DemoBundle\Security\Firewall\WsseListener
            arguments: [@security.context, @security.authentication.manager]


    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <services>
            <service id="wsse.security.authentication.provider"
              class="Acme\DemoBundle\Security\Authentication\Provider\WsseProvider" public="false">
                <argument /> <!-- User Provider -->
                <argument>%kernel.cache_dir%/security/nonces</argument>
            </service>

            <service id="wsse.security.authentication.listener"
              class="Acme\DemoBundle\Security\Firewall\WsseListener" public="false">
                <argument type="service" id="security.context"/>
                <argument type="service" id="security.authentication.manager" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setDefinition('wsse.security.authentication.provider',
          new Definition(
            'Acme\DemoBundle\Security\Authentication\Provider\WsseProvider',
            array('', '%kernel.cache_dir%/security/nonces')
        ));

        $container->setDefinition('wsse.security.authentication.listener',
          new Definition(
            'Acme\DemoBundle\Security\Firewall\WsseListener', array(
              new Reference('security.context'),
              new Reference('security.authentication.manager'))
        ));

これでサービスを定義したので、セキュリティコンテキストに作成したファクトリを指定しましょう。現段階では、ファクトリは、個々のコンフィギュレーションファイルでインクルードされる必要があります。ファイルを作成する必要があります。そして、コンフィギュレーションの ``factories`` キーを使用してインポートします。

.. code-block:: xml

    <!-- src/Acme/DemoBundle/Resources/config/security_factories.xml -->
    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

        <services>
            <service id="security.authentication.factory.wsse"
              class="Acme\DemoBundle\DependencyInjection\Security\Factory\WsseFactory" public="false">
                <tag name="security.listener.factory" />
            </service>
        </services>
    </container>

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
          factories:
            - "%kernel.root_dir%/../src/Acme/DemoBundle/Resources/config/security_factories.xml"

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <factories>
              "%kernel.root_dir%/../src/Acme/DemoBundle/Resources/config/security_factories.xml
            </factories>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'factories' => array(
              "%kernel.root_dir%/../src/Acme/DemoBundle/Resources/config/security_factories.xml"
            ),
        ));

これで WSSE 保護下にアプリケーションを定義できるようになりました。

.. code-block:: yaml

    security:
        firewalls:
            wsse_secured:
                pattern:   /api/.*
                wsse:      true

おめでとう！カスタムセキュリティ認証プロバイダを作成することができました。

おまけ
--------------

WSSE 認証プロバイダをもっとエキサイティングにしてみましょう。

コンフィギュレーション
~~~~~~~~~~~~~

セキュリティコンフィギュレーション内の ``wsse`` キー以下にカスタムオプションを追加することができます。例として、デフォルトでは５分のヘッダアイテムの期限切れの時間を変更しましょう。この時間を設定可能にすれば、異なるファイアーウォールが異なるタイムアウトの時間を保持することができます。

まず ``wsseFactory`` を編集して addConfiguration`` メソッド内に新しいオプションを定義します。

.. code-block:: php

    class WsseFactory implements SecurityFactoryInterface
    {
        # ...

        public function addConfiguration(NodeDefinition $node)
        {
          $node
            ->children()
              ->scalarNode('lifetime')->defaultValue(300)
            ->end()
          ;
        }
    }

これでファクトリの ``create`` メソッド内に ``$config`` 引数が 'lifetime' キーを含むことができます。 コンフィギュレーションでセットしなければ、５分(３００秒)にセットになります。これを使用するために、認証プロバイダにこの引数を渡してください。

.. code-block:: php

    class WsseFactory implements SecurityFactoryInterface
    {
        public function create(ContainerBuilder $container, $id, $config, $userProvider, $defaultEntryPoint)
        {
            $providerId = 'security.authentication.provider.wsse.'.$id;
            $container
                ->setDefinition($providerId,
                  new DefinitionDecorator('wsse.security.authentication.provider'))
                ->replaceArgument(0, new Reference($userProvider))
                ->replaceArgument(2, $config['lifetime'])
            ;
            // ...
        }
        // ...
    }

.. note::

    ``wsse.security.authentication.provider`` サービスコンフィギュレーションに第三引数を追加する必要もあります。ブランクでもいいですが、ファクトリ内でライフタイムが設定されます。これで ``WsseProvider`` クラスは、コンストラクタに第三引数を受け取る必要があります。ハードコードされた３００秒ではなく、第三引数が使用されることになります。この２つのステップは次の通りです。

各 ``wsse`` リクエストのライフタイムは、これで設定可能になり、ファイアーウォールごとに値をセットすることができるようになりました。

.. code-block:: yaml

    security:
        firewalls:
            wsse_secured:
                pattern:   /api/.*
                wsse:      { lifetime: 30 }

残りはあたな自身が拡張してみてください。どんな関連するコンフィギュレーションアイテムもファクトリで定義することができ、コンテナ内で他のクラスに渡すことができます。

.. _`WSSE`: http://www.xml.com/pub/a/2003/12/17/dive.html
.. _`nonce`: http://en.wikipedia.org/wiki/Cryptographic_nonce

.. 2011/11/18 ganchiku e04aa9a90bc1e3d0ea808a28ac96355e25d6e724

