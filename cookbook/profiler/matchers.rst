.. note::

 * 対象バージョン：2.6
 * 翻訳更新日：2014/12/01

.. index::
    single: Profiling; Matchers

プロファイラーを条件に応じて有効化する Matchers の使い方
========================================================

デフォルトではプロファイラーは開発環境でしか有効になっていません。
しかし、開発者が本番環境でもプロファイラーを見たいということは想像できます。
管理者としてログインした場合のみプロファイラーを表示させたいという状況もあるでしょう。
Matchers を使うことで状況に応じてプロファイラーを有効にすることが可能です。

built-in Matcher を使う
--------------------------

Symfony は Path と IPアドレスにマッチすることができる
:class:`built-in matcher <Symfony\\Component\\HttpFoundation\\RequestMatcher>`
を提供しています。
例えば、もし ``168.0.0.1`` という IP アドレスでアクセスした場合のみプロファイラーを表示させたい場合には
このような設定を使います。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            profiler:
                matcher:
                    ip: 168.0.0.1

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:profiler
                ip="168.0.0.1"
            />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'ip' => '168.0.0.1',
            ),
        ));

プロファイラーが有効であるべきパスを ``path`` オプションに定義することも可能です。
例えば、 ``^/admin/`` に設定することで ``/admin/`` という URL のみプロファイラーを有効にできます。

カスタマイズした Matcher を使う
-------------------------

カスタマイズした Matcher を作ることも可能です。
これはプロファイラーを有効にするべきか否かをチェックするサービスです。
:class:`Symfony\\Component\\HttpFoundation\\RequestMatcherInterface`
を実装したクラスでサービスを作ります。
このインターフェースには
:method:`Symfony\\Component\\HttpFoundation\\RequestMatcherInterface::matches`.
というメソッドの実装が必要です。
このメソッドは、プロファイラーが無効の場合は False を、有効な場合は True を返します。

``ROLE_SUPER_ADMIN`` がログインした場合にプロファイラーを有効にするときはこのように実装します::

    // src/Acme/DemoBundle/Profiler/SuperAdminMatcher.php
    namespace Acme\DemoBundle\Profiler;

    use Symfony\Component\Security\Core\SecurityContext;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\RequestMatcherInterface;

    class SuperAdminMatcher implements RequestMatcherInterface
    {
        protected $securityContext;

        public function __construct(SecurityContext $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        public function matches(Request $request)
        {
            return $this->securityContext->isGranted('ROLE_SUPER_ADMIN');
        }
    }

そしてサービスを設定します:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            acme_demo.profiler.matcher.super_admin.class: Acme\DemoBundle\Profiler\SuperAdminMatcher

        services:
            acme_demo.profiler.matcher.super_admin:
                class: "%acme_demo.profiler.matcher.super_admin.class%"
                arguments: ["@security.context"]

    .. code-block:: xml

        <parameters>
            <parameter
                key="acme_demo.profiler.matcher.super_admin.class"
            >Acme\DemoBundle\Profiler\SuperAdminMatcher</parameter>
        </parameters>

        <services>
            <service id="acme_demo.profiler.matcher.super_admin"
                class="%acme_demo.profiler.matcher.super_admin.class%">
                <argument type="service" id="security.context" />
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter(
            'acme_demo.profiler.matcher.super_admin.class',
            'Acme\DemoBundle\Profiler\SuperAdminMatcher'
        );

        $container->setDefinition('acme_demo.profiler.matcher.super_admin', new Definition(
            '%acme_demo.profiler.matcher.super_admin.class%',
            array(new Reference('security.context'))
        );

これでサービスが登録されました。最後の仕上げにこのサービスを Matcher として利用し
プロファイラーを設定しましょう:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            profiler:
                matcher:
                    service: acme_demo.profiler.matcher.super_admin

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <!-- ... -->
            <framework:profiler
                service="acme_demo.profiler.matcher.super_admin"
            />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'profiler' => array(
                'service' => 'acme_demo.profiler.matcher.super_admin',
            ),
        ));
