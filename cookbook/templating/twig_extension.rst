.. index::
   single: Twig extensions

.. note::

    * 翻訳更新日：2013/06/12

カスタムTwig拡張の書き方
====================================

カスタムTwig拡張を書く主な動機は、国際化のようなよく使うコードを再利用可能なクラスにすることです。
Twig拡張には、タグ、フィルタ、テスト、演算子、グローバル変数、関数、ノードビジターを定義することができます。

拡張を書くことで、コンパイル時のコードと実行時のコードをきれいに分離することもできるようになります。あなたのコードも少し速くなります。

.. tip::

    自作のTwig拡張を書く前に、\ `Twigの公式拡張レポジトリ`_\ も見てみてください。

Twig拡張クラスの作成
--------------------------

.. note::

    このクックブックでは、Twig 1.12以降を対象としてカスタムTwig拡張の書き方を説明しています。もっと古いバージョンのTwigを使用している場合は `Twig extensions documentation legacy`_ を参照してください。

カスタムの機能を使うためには、まずTwig拡張のクラスを作成する必要があります。
一例として、数値を価格としてフォーマットするための価格フィルタを作るとします。
::

    // src/Acme/DemoBundle/Twig/AcmeExtension.php
    namespace Acme\DemoBundle\Twig;

    class AcmeExtension extends \Twig_Extension
    {
        public function getFilters()
        {
            return array(
                new \Twig_SimpleFilter('price', array($this, 'priceFilter')),
            );
        }

        public function priceFilter($number, $decimals = 0, $decPoint = '.', $thousandsSep = ',')
        {
            $price = number_format($number, $decimals, $decPoint, $thousandsSep);
            $price = '$'.$price;

            return $price;
        }

        public function getName()
        {
            return 'acme_extension';
        }
    }

.. tip::

    フィルタだけでなく、カスタムの\ `関数`_\ を作ったり、\ `グローバル変数`_\ を登録したりすることもできます。

Twig拡張をサービスとして登録
----------------------------------

新しいTwig拡張をサービスコンテナに登録しましょう。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            acme.twig.acme_extension:
                class: Acme\DemoBundle\Twig\AcmeExtension
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <services>
            <service id="acme.twig.acme_extension" class="Acme\DemoBundle\Twig\AcmeExtension">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->register('acme.twig.acme_extension', '\Acme\DemoBundle\Twig\AcmeExtension')
            ->addTag('twig.extension');

.. note::

   Twig拡張は遅延ロードされないので、注意してください。
   これは、もしrequestサービスに依存していた場合に、\ **CircularReferenceException** や **ScopeWideningInjectionException** がかなりの確率で発生するということを意味します。
   より詳細に知りたければ :doc:`/cookbook/service_container/scopes` を読んでください。

カスタムTwig拡張を使う
----------------------

新しく作成したカスタムTwig拡張の機能は、通常の機能とまったく変わりなく使用できます。

.. code-block:: jinja

    {# $5,500.00 を表示 #}
    {{ '5500'|price }}

カスタムのフィルタに引数を渡す例です。

.. code-block:: jinja

    {# $5500,2516 を表示 #}
    {{ '5500.25155'|price(4, ',', '') }}

もっと詳しく学ぶ
----------------

Twig拡張についてもっと詳しく知りたい場合は、\ `Twig extensions documentation`_ を読んでください。

.. _`Twigの公式拡張レポジトリ`: https://github.com/fabpot/Twig-extensions
.. _`Twig extensions documentation`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`グローバル変数`: http://twig.sensiolabs.org/doc/advanced.html#id1
.. _`関数`: http://twig.sensiolabs.org/doc/advanced.html#id2
.. _`Twig extensions documentation legacy`: http://twig.sensiolabs.org/doc/advanced_legacy.html#creating-an-extension


.. 2013/06/12 77web abe3406e8180fed1d019d5c0246d75d55ed22c8c
