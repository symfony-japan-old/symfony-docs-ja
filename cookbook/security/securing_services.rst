アプリケーション内でサービスやメソッドをセキュアにする方法
=======================================================

セキュリティの章では、サービスコンテナの ``security.context`` サービスを要求して、ユーザの権限を調べることで、コントローラをセキュアにする(:ref:`secure a controller<book-security-securing-controller>`)方法を参照することができます。
::

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    // ...

    public function helloAction($name)
    {
        if (false === $this->get('security.context')->isGranted('ROLE_ADMIN')) {
            throw new AccessDeniedException();
        }

        // ...
    }

同じような方法を使用して ``security.context`` サービスを注入する(inject)することで、 *全て* のサービスをセキュアにすることができます。サービスの依存(dependencies)注入(inject)に関する、より一般的なイントロダクションとして、ガイドブックの :doc:`/book/service_container` の章を参照してください。例として、メール送信を行う ``ROLE_NEWSLETTER_ADMIN`` 権限を持ったユーザのみしか使用できない ``NewsletterManager`` クラスがあるとしましょう。セキュリティを追加する前は、このクラスは次のようになっています。

.. code-block:: php

    namespace Acme\HelloBundle\Newsletter;

    class NewsletterManager
    {

        public function sendNewsletter()
        {
            // where you actually do the work
        }

        // ...
    }

``sendNewsletter()`` メソッドが呼ばれた際に、ユーザの権限を調べるようにする必要があります。最初のステップとして、オブジェクトに `security.context`` サービスを注入(inject)します。セキュリティチェックとして *機能する* 必要があるので、コンストラクタへの注入(injection)の理想的な候補として ``NewsletterManager`` 内で利用可能なセキュリティコンテキストオブジェクトを保証するようします。
::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Security\Core\SecurityContextInterface;

    class NewsletterManager
    {
        protected $securityContext;

        public function __construct(SecurityContextInterface $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        // ...
    }

サービスコンフィギュレーションで、サービスを注入(inject)することができます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@security.context]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="security.context"/>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('security.context'))
        ));

注入された(injected)サービスは、 ``sendNewsletter()`` メソッドが呼ばれたときのセキュリティチェックとして機能として使用することができます。
::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    // ...

    class NewsletterManager
    {
        protected $securityContext;

        public function __construct(SecurityContextInterface $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        public function sendNewsletter()
        {
            if (false === $this->securityContext->isGranted('ROLE_NEWSLETTER_ADMIN')) {
                throw new AccessDeniedException();
            }

            //--
        }

        // ...
    }

ユーザが ``ROLE_NEWSLETTER_ADMIN`` 権限を持っていなければ、ログインを促します。

アノテーションを使用してメソッドをセキュアにする
----------------------------------

オプションの `JMSSecurityExtraBundle` バンドルを使用することで、アノテーションを使ってどんなサービスのメソッド呼び出しもセキュアにすることができます。このバンドルは Symfony2 Standard Distribution に付いてきます。

アノテーションの機能を有効にするには、セキュアにしたいサービスに ``security.secure_service`` タグ  :ref:`tag<book-service-container-tags>` を付けてください(下のサイドバー  :ref:`sidebar<securing-services-annotations-sidebar>` を参考に全てのサービスの機能を自動的に有効にすることもできます)。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        # ...

        services:
            newsletter_manager:
                # ...
                tags:
                    -  { name: security.secure_service }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <!-- ... -->

        <services>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <!-- ... -->
                <tag name="security.secure_service" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition(
            '%newsletter_manager.class%',
            array(new Reference('security.context'))
        ));
        $definition->addTag('security.secure_service');
        $container->setDefinition('newsletter_manager', $definition);

これでアノテーションを使用して上記と同じ実装をすることができます。
::

    namespace Acme\HelloBundle\Newsletter;

    use JMS\SecurityExtraBundle\Annotation\Secure;
    // ...

    class NewsletterManager
    {

        /**
         * @Secure(roles="ROLE_NEWSLETTER_ADMIN")
         */
        public function sendNewsletter()
        {
            //--
        }

        // ...
    }

.. note::

    セキュリティチェックとして機能するクラスのプロクシクラスを作成すれば、アノテーションは動作するようになります。アノテーションは、public と protected なメソッドには使用することができますが、 private なメソッドや final 指定のメソッドには使用することはできません。

``JMSSecurityExtraBundle`` を使用すれば、パラメターとメソッドの返り値をセキュアにすることができます。詳細は、  `JMSSecurityExtraBundle`_ のドキュメントを参照してください。

.. _securing-services-annotations-sidebar:

.. sidebar:: Activating the Annotations Functionality for all Services

    上記のようにサービスのメソッドをセキュアにするには、個々のサービス毎にタグ付けても実現できますが、一度に *全て* のサービスの機能を有効にすることもできます。そうするには ``secure_all_services`` コンフィギュレーションオプションに ``true`` を指定してください。

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            jms_security_extra:
                # ...
                secure_all_services: true

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <srv:container xmlns="http://symfony.com/schema/dic/security"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:srv="http://symfony.com/schema/dic/services"
                xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

                <jms_security_extra secure_controllers="true" secure_all_services="true" />

            </srv:container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('jms_security_extra', array(
                // ...
                'secure_all_services' => true,
            ));

    この方法のディスアドバンテージは、有効にした時に、定義したサービスの数によっては、初期ページがとても遅くなることがあることです。

.. _`JMSSecurityExtraBundle`: https://github.com/schmittjoh/JMSSecurityExtraBundle

.. 2011/11/08 ganchiku f3462fda31041f75fd462226525b81a3f3fa451d

