.. 2011/07/04 jptomo 0bc45c95fbe796736cf441028fad9088f27ece00

.. index::
   pair: Twig; Configuration Reference

TwigBundle 設定
==================================

.. configuration-block::

    .. code-block:: yaml

        twig:
            form:
                resources:

                    # 初期値
                    - div_layout.html.twig

                    # 設定例:
                    - MyBundle::form.html.twig
            globals:

                # 設定例:
                foo:                 "@bar"
                pi:                  3.14

                # プロトタイプ
                key:
                    id:                   ~
                    type:                 ~
                    value:                ~
            autoescape:           ~
            base_template_class:  ~ # 設定例: Twig_Template
            cache:                %kernel.cache_dir%/twig
            charset:              %kernel.charset%
            debug:                %kernel.debug%
            strict_variables:     ~
            auto_reload:          ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/doctrine/twig-1.0.xsd">

            <twig:config auto-reload="%kernel.debug%" autoescape="true" base-template-class="Twig_Template" cache="%kernel.cache_dir%/twig" charset="%kernel.charset%" debug="%kernel.debug%" strict-variables="false">
                <twig:form>
                    <twig:resource>MyBundle::form.html.twig</twig:resource>
                </twig:form>
                <twig:global key="foo" id="bar" type="service" />
                <twig:global key="pi">3.14</twig:global>
            </twig:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'MyBundle::form.html.twig',
                )
             ),
             'globals' => array(
                 'foo' => '@bar',
                 'pi'  => 3.14,
             ),
             'auto_reload'         => '%kernel.debug%',
             'autoescape'          => true,
             'base_template_class' => 'Twig_Template',
             'cache'               => '%kernel.cache_dir%/twig',
             'charset'             => '%kernel.charset%',
             'debug'               => '%kernel.debug%',
             'strict_variables'    => false,
        ));
