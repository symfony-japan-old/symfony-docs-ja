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


コントローラのアノテーション
-----------------------------

アノテーションはコントローラやルートやキャッシュの設定を簡単にする素晴しい方法です。

たとえアノテーションがPHPの標準機能じゃないとしても、それでもそれは標準的なSymfony2の設定方法を越えるいくつかの利点があります。:

* コードと設定が同じ場所にあります。(コントローラの場合)
* 簡単に学べて使えます。
* 簡潔に書けます。
* 小さなコントローラを作れます。(モデルからデータを取得するだけのコントローラみたいな)

.. tip::

   If you use view classes, annotations are a great way to avoid creating
   view classes for simple and common use cases.
   もしビュークラスを使うなら、アノテーションは簡単で一般的な使用事例のためにビュークラスを作ることを避ける素晴しい方法です。

以下のアノテーションがバンドルで定義されるものです。:

.. toctree::
   :maxdepth: 1

   annotations/routing
   annotations/converters
   annotations/view
   annotations/cache

この例はアクションの中で利用できる全てのアノテーションを全て設定しています。::

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

``showAction`` はコンベンションを以下のようにでき、いくつかのアノテーションを省略することができます。::


    /**
     * @Route("/{id}")
     * @Cache(smaxage="15")
     */
    public function showAction(Post $post)
    {
    }

.. _`SensioFrameworkExtraBundle`: https://github.com/sensio/SensioFrameworkExtraBundle
.. _`ダウンロード`: http://github.com/sensio/SensioFrameworkExtraBundle

.. 2012/10/13 vectorxenon 9e0f169275102b62f9e16f1b7bfc8d596bc242ca
