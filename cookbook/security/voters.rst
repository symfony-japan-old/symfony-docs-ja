.. index::
   single: Security, Voters

IPアドレスのブラックリストの独自 Voter の実装方法
=================================================

Symfony2 のセキュリティコンポーネントは、ユーザ認証のための複数のレイヤーを用意しています。 `voter` と呼ばれるレイヤーは、その１つです。 voter はユーザがアプリケーションに接続できる権利があるかのチェックを行うクラスです。例えば、 Symfony2 は、ユーザが完全に認証されているか、また、必要な権限を保持しているかといったことをチェックするレイヤーを提供します。

フレームワークによる処理ではなく、特定のケースを処理するためのカスタム化された voter が役に立つこともあります。このセクションでは、 IP アドレスに基づきユーザをブラックリストに入れるための voter の作り方を学びましょう

Voter インタフェース
--------------------

カスタム Voter は、次の３つのメソッドを必要とする :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface` インタフェースを実装する必要があります:

.. code-block:: php

    interface VoterInterface
    {
        function supportsAttribute($attribute);
        function supportsClass($class);
        function vote(TokenInterface $token, $object, array $attributes);
    }


``supportsAttribute()`` メソッドは、 voter が権限や ACL といったユーザの属性をサポートするかチェックします。

``supportsClass()`` メソッドは、 voter が現在のユーザのトークンクラスをサポートするかチェックします。

``vote()`` メソッドは、ビジネスロジックを実装する必要があり、そこでユーザがアクセス可能か証明します。このメソッドは、次の値のいずれかを返す必要があります。

* ``VoterInterface::ACCESS_GRANTED``: ユーザがアプリケーションにアクセス可能である
* ``VoterInterface::ACCESS_ABSTAIN``: voter では、 ユーザがアクセス可能か否かが判断できない
* ``VoterInterface::ACCESS_DENIED``: ユーザがアプリケーションにアクセス不可能である

この例では、ユーザの IP アドレスがブラックリストのアドレス群を調べます。ユーザの IP がブラックリスト内にあれば、 ``VoterInterface::ACCESS_DENIED`` を返し、そうでなければ、 ``VoterInterface::ACCESS_ABSTAIN`` を返します。つまり、この voter の目的は、実際のアクセスを与えることではなく、アクセスの拒否のみとします。

カスタム Voter の作成方法
-------------------------

ユーザを IP に基づいてブラックリストに入れるには、 ``request`` サービスを使用してブラックリストの IP アドレスの集合と比較します。

.. code-block:: php

    namespace Acme\DemoBundle\Security\Authorization\Voter;

    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Symfony\Component\Security\Core\Authorization\Voter\VoterInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;

    class ClientIpVoter implements VoterInterface
    {
        public function __construct(ContainerInterface $container, array $blacklistedIp = array())
        {
            $this->container     = $container;
            $this->blacklistedIp = $blacklistedIp;
        }

        public function supportsAttribute($attribute)
        {
            // ユーザ属性は調べないので、 true を返します
            return true;
        }

        public function supportsClass($class)
        {
            // voter はトークンクラスの全てをサポートするので、 true を返します
            return true;
        }

        function vote(TokenInterface $token, $object, array $attributes)
        {
            $request = $this->container->get('request');
            if (in_array($this->request->getClientIp(), $this->blacklistedIp)) {
                return VoterInterface::ACCESS_DENIED;
            }

            return VoterInterface::ACCESS_ABSTAIN;
        }
    }

これで voter ができました。次のステップは、 voter をセキュリティレイヤーに注入する(inject)ことです。これは、サービスコンテナを介して簡単に行うことができます。

Voter をサービスとして宣言する
------------------------------

Voter をセキュリティレイヤーに注入する(inject)には、 Voter をサービスとして宣言して、 "security.voter" としてタグ付けする必要があります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/AcmeBundle/Resources/config/services.yml

        services:
            security.access.blacklist_voter:
                class:      Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter
                arguments:  [@service_container, [123.123.123.123, 171.171.171.171]]
                public:     false
                tags:
                    -       { name: security.voter }

    .. code-block:: xml

        <!-- src/Acme/AcmeBundle/Resources/config/services.xml -->

        <service id="security.access.blacklist_voter"
                 class="Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter" public="false">
            <argument type="service" id="service_container" strict="false" />
            <argument type="collection">
                <argument>123.123.123.123</argument>
                <argument>171.171.171.171</argument>
            </argument>
            <tag name="security.voter" />
        </service>

    .. code-block:: php

        // src/Acme/AcmeBundle/Resources/config/services.php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition(
            'Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter',
            array(
                new Reference('service_container'),
                array('123.123.123.123', '171.171.171.171'),
            ),
        );
        $definition->addTag('security.voter');
        $definition->setPublic(false);

        $container->setDefinition('security.access.blacklist_voter', $definition);

.. tip::

   このコンフィギュレーションファイルをメインのアプリケーションファイル( ``app/config/config.yml`` など)からインポートすることを忘れないでください。詳細は、 :ref:`service-container-imports-directive` を参照してください。より一般的なサービスの定義については、ドキュメントの :doc:`/book/service_container` 章を参照してください。

アクセス可否の決定戦略を変更する
--------------------------------

新しい voter の効力を有効にするために、デフォルトのアクセス決定戦略を変更する必要があります。デフォルトでは、 *いずれかの* voter がアクセスを許可していれば、良いことになっています。

ここでのケースでは、 ``unanimous`` 戦略を選択しましょう。デフォルトの ``affirmative`` 戦略と異なり、 ``unanimous`` 戦略は voter １つでもアクセスを拒否すれば(例えば ``ClientIpVoter`` )、ユーザにアクセスが許可されません。

アプリケーションのコンフィギュレーションファイルの ``access_decision_manager`` セクションをデフォルト値から次のようにオーバーライドしましょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_decision_manager:
                # Strategy can be: affirmative, unanimous or consensus
                strategy: unanimous

できました。これで、ユーザがアクセスがあるかどうかを決定する歳に、新しい Voter はブラックリスト IP リストに入っているユーザを全てアクセス拒否するようになりました。

.. 2011/11/17 ganchiku cdde9366687364639706b50e1440ac962a696b75
