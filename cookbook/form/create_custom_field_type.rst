.. index::
   single: Form; Custom field type

カスタムフォームフィールドタイプの作成方法
==========================================

Symfony には、フォーム作成に関するたくさんのコアフィールドタイプが付いてきます。しかし、特定の目的のためにカスタムフィールドタイプを作成したいときもあります。このレシピでは、既にある選択フィールドをベースとした性別の選択を行うフィールドを作成する必要がある、ということを想定します。このセクションでは、フィールドの定義方法、フィールドのレイアウトのカスタマイズ方法、フィールドのアプリケーションへの登録方法を説明します。

フィールドタイプの定義
-----------------------

カスタムフィールドタイプを作成するには、まずそのフィールドを表すクラスを作成する必要があります。今回は、そのフィールドタイプを保持するクラスを ``GenderType`` とします。そして、そのファイルはフォームフィールドのデフォルトの位置である ``<BundleName>\Form\Type`` に格納されるとします。フィールドに :class:`Symfony\\Component\\Form\\AbstractType` を拡張させるのを忘れないでください。
::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class GenderType extends AbstractType
    {
        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'choices' => array(
                    'm' => 'Male',
                    'f' => 'Female',
                )
            ));
        }

        public function getParent(array $options)
        {
            return 'choice';
        }

        public function getName()
        {
            return 'gender';
        }
    }

.. tip::

    このファイルの配置場所は、重要ではありません。 ``Form\Type`` ディレクトリは習慣です。

``getParent`` メソッドの返す値は ``choice`` を拡張していることを示しています。
つまり、デフォルトではchoiceフィールドタイプのロジックとレンダリングを継承するという意味です。ロジックを見るには `ChoiceType`_ クラスを見てみてください。特に重要なのは次の3つのメソッドです。

* ``buildForm()`` - 全てのフィールドタイプに ``buildForm`` メソッドがありますが、ここでフィールドを設定したり構築したりすることができます。自作のフォームを作るときと同じメソッドで、同じように動作します。

* ``buildView()`` - このメソッドはフィールドをテンプレートにレンダリングする際に、追加の変数をセットするのに使います。例えば、 `ChoiceType`_ では ``multiple`` という変数がテンプレートで ``select`` 要素に ``multiple`` 属性を追加するかどうかを判断するのに使います。詳しくは `Creating a Template for the Field`_ を見てください。

* ``getDefaultOptions()`` - このメソッドは、フィールドタイプの ``buildForm()`` や ``buildView()`` で使うことができるオプションを定義します。全てのフィールドタイプに共通のオプションもたくさんありますが( `FieldType`_ を見てください)、必要なら新しいオプションをここで追加することができます。

.. tip::

    たくさんのフィールドを使ったフィールドを作ろうとしている場合は、"親"のタイプを ``form`` 又は ``form`` を拡張した何かにしてください。
    また、親のフィールドタイプから子のフィールドタイプのビューを制御したい場合は、 ``finishView()`` メソッドを使ってください。

``getName()`` メソッドは、アプリケーションの中で一意な識別子を返します。この識別子はフィールドタイプのレンダリングをカスタマイズする場合など、多くの場所で使います。

いま作っているフィールドのゴールは、性別の選択ができるようにchoiceフィールドタイプを拡張することです。
``choices`` を選択可能な性別のリストに固定することで実現できます。

フィールドのテンプレートの作成
---------------------------------

全てのフィールドタイプは、テンプレートフラグメントを使ってレンダリングされ、 ``getName()`` を使って識別されます。
詳しくは :ref:`cookbook-form-customization-form-themes` を読んでください。

今回は、親フィールドが ``choice`` なので、カスタムフィールドタイプは自動的に ``choice`` と同じようにレンダリングされ、カスタマイズする必要はありません。
しかし、例として使うために、フィールドが"expanded"の場合(selectでなくradioやcheckboxを使う場合)に、 ``ul` 要素を使ってレンダリングしたいと考えてみましょう。
フォームテーマのテンプレート(詳細は上のリンクを見てください)に、 ``gender_widget`` ブロックを作ってください。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}
        {% block gender_widget %}
            {% spaceless %}
                {% if expanded %}
                    <ul {{ block('widget_container_attributes') }}>
                    {% for child in form %}
                        <li>
                            {{ form_widget(child) }}
                            {{ form_label(child) }}
                        </li>
                    {% endfor %}
                    </ul>
                {% else %}
                    {# choice widgetにそのままselectタグを出力させる #}
                    {{ block('choice_widget') }}
                {% endif %}
            {% endspaceless %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/DemoBundle/Resources/views/Form/gender_widget.html.twig -->
        <?php if ($expanded) : ?>
            <ul <?php $view['form']->block($form, 'widget_container_attributes') ?>>
            <?php foreach ($form as $child) : ?>
                <li>
                    <?php echo $view['form']->widget($child) ?>
                    <?php echo $view['form']->label($child) ?>
                </li>
            <?php endforeach ?>
            </ul>
        <?php else : ?>
            <!-- choice widgetにそのままselectタグを出力させる -->
            <?php echo $view['form']->renderBlock('choice_widget') ?>
        <?php endif ?>

.. note::

    正しいウィジェット接頭辞が使われていることを確認してください。今回の例では、名前は、 ``getName`` が返す値によれば、 ``gender_widget`` であるべきです。さらに、メインのコンフィギュレーションファイルにおいてカスタムフォームテンプレートを設定して、全てのフォームで使用できるようにしておかなければなりません。

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            twig:
                form:
                    resources:
                        - 'AcmeDemoBundle:Form:fields.html.twig'

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <twig:config>
                <twig:form>
                    <twig:resource>AcmeDemoBundle:Form:fields.html.twig</twig:resource>
                </twig:form>
            </twig:config>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('twig', array(
                'form' => array(
                    'resources' => array(
                        'AcmeDemoBundle:Form:fields.html.twig',
                    ),
                ),
            ));

フィールドタイプの使用
----------------------

ここまでで、フォーム内でカスタムフィールドタイプのインスタンスを作成すれば、すぐにカスタムフィールドタイプを使用できるようになりました。
::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('gender_code', new GenderType(), array(
                'empty_value' => '性別を選んでください',
            ));
        }
    }

``GenderType()`` はとてもシンプルなのでこれで動作します。しかし、 gender コードが設定ファイルやデータベースに格納されていたらどうでしょうか？次のセクションでは、複雑なフィールドタイプでどうやってこの問題を解決するのか説明します。

.. _form-cookbook-form-field-service:

フィールドタイプをサービスとして作成
-------------------------------------

ここまでこの記事では、カスタムフィールドタイプがとてもシンプルであることを想定していました。しかし、コンフィギュレーション、データベース接続、他のサービスなどにアクセスが必要になったときは、　
タイプをサービスとして登録するのが良いでしょう。例として、 gender パラメータをコンフィギュレーションに格納するとしましょう。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            acme_demo.form.type.gender:
                class: Acme\DemoBundle\Form\Type\GenderType
                arguments:
                    - "%genders%"
                tags:
                    - { name: form.type, alias: gender }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <service id="acme_demo.form.type.gender" class="Acme\DemoBundle\Form\Type\GenderType">
            <argument>%genders%</argument>
            <tag name="form.type" alias="gender" />
        </service>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition('acme_demo.form.type.gender', new Definition(
                'Acme\DemoBundle\Form\Type\GenderType',
                array('%genders%')
            ))
            ->addTag('form.type', array(
                'alias' => 'gender',
            ))
        ;

.. tip::

    サービス設定ファイルがインポートされるのを確認してください。詳細は、 :ref:`service-container-imports-directive` を参照してください。

``alias`` タグが、以前定義した ``getName`` メソッドの返り値と一致しているのを確認してください。カスタムフィールドタイプを使用する際に、このことが重要であるということがわかります。まず最初に、gender の設定を引数として受け取ることになる ``__construct`` を``GenderType`` に追加してください。
::

    // src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    // ...

    // ...
    class GenderType extends AbstractType
    {
        private $genderChoices;

        public function __construct(array $genderChoices)
        {
            $this->genderChoices = $genderChoices;
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'choices' => $this->genderChoices,
            ));
        }

        // ...
    }

できました！これで ``GenderType`` はコンフィギュレーションパラメータによって動くようになり、サービスとして登録されました。更に、コンフィギュレーションで ``form.type`` エイリアスを使用したのでフィールドを使うのがより簡単になりました。
::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;

    // ...

    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('gender_code', 'gender', array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

このように、新しいインスタンスを初期化するのではなく、サービスコンフィギュレーションの ``gender`` 内で使われているエイリアスで参照できるようになりました。

.. _`ChoiceType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/ChoiceType.php
.. _`FieldType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/FieldType.php

.. 2012/01/11 ganchiku 91c6267021ec47d8baed3eaf76ffca7826221e35
.. 2013/06/12 77web 0826e3389949dd97c7cd813725a94819f572d5d7
