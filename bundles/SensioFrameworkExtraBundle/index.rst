SensioFrameworkExtraBundle
==========================

Symfony2標準の ``FrameworkBundle`` は基本的でありながら頑丈で柔軟なMVCフレームワークを供給します。
`SensioFrameworkExtraBundle`_ はそれにコンベンションとアノテーションの追加を施します。
それはより簡潔なコントローラを可能にします。

インストール
------------

SensioFrameworkExtraBundleを `ダウンロード`_ し、 ``Sensio\Bundle\`` 名前空間に追加します。
そして、他のバンドルと同じようにKernelクラスに登録します。::

    public function registerBundles()
    {
        $bundles = array(
            ...

            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
        );

        ...
    }

設定
-------------

全ての機能はKernelクラスに登録した時に標準で有効になり、利用できるようになります。

標準の設定内容は以下のようになります:

.. configuration-block::

    .. code-block:: yaml

        sensio_framework_extra:
            router:  { annotations: true }
            request: { converters: true }
            view:    { annotations: true }
            cache:   { annotations: true }

    .. code-block:: xml

        <!-- xmlns:sensio-framework-extra="http://symfony.com/schema/dic/symfony_extra" -->
        <sensio-framework-extra:config>
            <router annotations="true" />
            <request converters="true" />
            <view annotations="true" />
            <cache annotations="true" />
        </sensio-framework-extra:config>

    .. code-block:: php

        // load the profiler
        $container->loadFromExtension('sensio_framework_extra', array(
            'router'  => array('annotations' => true),
            'request' => array('converters' => true),
            'view'    => array('annotations' => true),
            'cache'   => array('annotations' => true),
        ));

1つあるいは複数の設定をfalseにすることで、いくつかのアノテーションとコンベンションを無効化することができます。


Annotations for Controllers
---------------------------

Annotations are a great way to easily configure your controllers, from the
routes to the cache configuration.

Even if annotations are not a native feature of PHP, it still has several
advantages over the classic Symfony2 configuration methods:

* Code and configuration are in the same place (the controller class);
* Simple to learn and to use;
* Concise to write;
* Makes your Controller thin (as its sole responsibility is to get data from
  the Model).

.. tip::

   If you use view classes, annotations are a great way to avoid creating
   view classes for simple and common use cases.

The following annotations are defined by the bundle:

.. toctree::
   :maxdepth: 1

   annotations/routing
   annotations/converters
   annotations/view
   annotations/cache

This example shows all the available annotations in action::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;

    /**
     * @Route("/blog")
     * @Cache(expires="tomorrow")
     */
    class AnnotController extends Controller
    {
        /**
         * @Route("/")
         * @Template
         */
        public function indexAction()
        {
            $posts = ...;

            return array('posts' => $posts);
        }

        /**
         * @Route("/{id}")
         * @Method("GET")
         * @ParamConverter("post", class="SensioBlogBundle:Post")
         * @Template("SensioBlogBundle:Annot:post.html.twig", vars={"post"})
         * @Cache(smaxage="15")
         */
        public function showAction(Post $post)
        {
        }
    }

As the ``showAction`` method follows some conventions, you can omit some
annotations::

    /**
     * @Route("/{id}")
     * @Cache(smaxage="15")
     */
    public function showAction(Post $post)
    {
    }

.. _`SensioFrameworkExtraBundle`: https://github.com/sensio/SensioFrameworkExtraBundle
.. _`ダウンロード`: http://github.com/sensio/SensioFrameworkExtraBundle

.. 2012/10/13 vectorxenon hogehoge
