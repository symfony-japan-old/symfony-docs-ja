.. index::
   single: Form; Custom field type

カスタムフォームフィールドタイプの作成方法
==========================================

Symfony には、フォーム作成に関するたくさんのコアフィールドタイプが付いてきます。しかし、特定の目的のためにカスタムフィールドタイプを作成したいときもあります。このレシピでは、既にある選択フィールドをベースとした性別の選択を行うフィールドを作成する必要がある、ということを想定します。このセクションでは、フィールドの定義方法、フィールドのレイアウトのカスタマイズ方法、フィールドのアプリケーションへの登録方法を説明します。

フィールドタイプの定義
-----------------------

カスタムフィールドタイプを作成するには、まずそのフィールドを表すクラスを作成する必要があります。今回は、そのフィールドタイプを保持するクラスを ``GenderType`` とします。そして、そのファイルはフォームフィールドのデフォルトの位置である ``<BundleName>\Form\Type`` に格納されるとします。フィールドが ``AbstractType`` を拡張するのを忘れないでください。
::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class GenderType extends AbstractType
    {
        public function getDefaultOptions(array $options)
        {
            return array(
                'choices' => array(
                    'm' => 'Male',
                    'f' => 'Female',
                )
            );
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

Here, the return value of the ``getParent`` function indicates that we're
extending the ``choice`` field type. This means that, by default, we inherit
all of the logic and rendering of that field type. To see some of the logic,
check out the `ChoiceType`_ class. There are three methods that are particularly
important:

* ``buildForm()`` - Each field type has a ``buildForm`` method, which is where
  you configure and build any field(s). Notice that this is the same method
  you use to setup *your* forms, and it works the same here.

* ``buildView()`` - This method is used to set any extra variables you'll
  need when rendering your field in a template. For example, in ``ChoiceType``,
  a ``multiple`` variable is set and used in the template to set (or not
  set) the ``multiple`` attribute on the ``select`` field. See `Creating a Template for the Field`_
  for more details.

* ``getDefaultOptions()`` - This defines options for your form type that
  can be used in ``buildForm()`` and ``buildView()``. There are a lot of
  options common to all fields (see `FieldType`_), but you can create any
  others that you need here.

.. tip::

    If you're creating a field that consists of many fields, then be sure
    to set your "parent" type as ``form`` or something that extends ``form``.
    Also, if you need to modify the "view" of any of your child types from
    your parent type, use the ``buildViewBottomUp()`` method.

The ``getName()`` method returns an identifier which is used to prevent conflicts
with other types. Other than needing to be unique, this method isn't very
important.

The goal of our field was to extend the choice type to enable selection of
a gender. This is achieved by fixing the ``choices`` to a list of possible
genders.

フィールドのテンプレートの作成
---------------------------------

Each field type is rendered by a template fragment, which is determined in
part by the value of your ``getName()`` method. For more information, see
:ref:`cookbook-form-customization-form-themes`.

In this case, since our parent field is ``choice``, we don't *need* to do
any work as our custom field type will automatically be rendered like a ``choice``
type. But for the sake of this example, let's suppose that when our field
is "expanded" (i.e. radio buttons or checkboxes, instead of a select field),
we want to always render it in a ``ul`` element. In your form theme template
(see above link for details), create a ``gender_widget`` block to handle this:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/Form/fields.html.twig #}
    {% use 'form_div_layout.html.twig' with choice_widget %}

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
            {# just let the choice widget render the select tag #}
            {{ block('choice_widget') }}
        {% endif %}
    {% endspaceless %}
    {% endblock %}

.. note::

    正しいウィジェット接頭辞が使われていることを確認してください。今回の例では、名前は、 ``getName`` によって返される値の ``gender_widget`` であるべきです。さらに、メインのコンフィギュレーションファイルは、カスタムフォームタイプを指定して、全てのフォームえ表示できるようにするべきです。

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeDemoBundle:Form:fields.html.twig'

フィールドタイプの使用
----------------------

これでフォームでカスタムフィールドタイプのインスタンスを作成すれば、すぐに使用できるようになりました。
::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    
    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('gender_code', new GenderType(), array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

``GenderType()`` がとてもシンプルなのでこれで動作します。しかし、 gender コードがコンフィギュレーションやデータベースに格納されていたらどうでしょうか？次のセクションでは、複雑なフィールドタイプでどうやってこの問題を解決するのか説明します。

フィールドタイプをサービスとして作成
-------------------------------------

ここまでこの記事では、カスタムフィールドタイプがとてもシンプルであることを想定していました。しかし、コンフィギュレーション、データベース接続、他のサービスなどにアクセスが必要になったときは、　
タイプをサービスとして登録するのが良いでしょう。例として、 gender パラメータをコンフィギュレーションに格納するとしましょう。

.. configuration-block::

    .. code-block:: yaml
    
        # app/config/config.yml
        parameters:
            genders:
                m: Male
                f: Female

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="genders" type="collection">
                <parameter key="m">Male</parameter>
                <parameter key="f">Female</parameter>
            </parameter>
        </parameters>

パラメータとして使用するに、カスタムフィールドタイプをサービスとして定義しましょう。 ``genders`` パラメータ値を ``__construct`` コンストラクタの第一引数として注入します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            form.type.gender:
                class: Acme\DemoBundle\Form\Type\GenderType
                arguments:
                    - "%genders%"
                tags:
                    - { name: form.type, alias: gender }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <service id="form.type.gender" class="Acme\DemoBundle\Form\Type\GenderType">
            <argument>%genders%</argument>
            <tag name="form.type" alias="gender" />
        </service>

.. tip::

    サービスファイルがインポートされるのを確認してください。詳細は、 :ref:`service-container-imports-directive` を参照してください。

``alias`` タグが、以前定義した ``getName`` メソッドの返り値と一致しているのを確認してください。カスタムフィールドタイプを使用する際に、このことが重要であるということがわかります。まず、 ``__construct`` の引数に、gender のコンフィギュレーションを受け取ることになる ``GenderType`` を追加してください。
::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;
    // ...

    class GenderType extends AbstractType
    {
        private $genderChoices;
        
        public function __construct(array $genderChoices)
        {
            $this->genderChoices = $genderChoices;
        }
    
        public function getDefaultOptions(array $options)
        {
            return array(
                'choices' => $this->genderChoices;
            );
        }
        
        // ...
    }

できました！これで ``GenderType`` はコンフィギュレーションパラメータによって動くようになり、サービスとして登録されます。コンフィギュレーションで ``form.type`` エイリアスを使用したのでフィールドの使用がとても簡単になりました。
::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;
    // ...

    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('gender_code', 'gender', array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

このように、新しいインスタンスを初期化するのではなく、サービスコンフィギュレーションの ``gender`` 内で使われているエイリアスで参照できます。

.. _`ChoiceType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/ChoiceType.php
.. _`FieldType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/FieldType.php

.. 2012/01/11 ganchiku 91c6267021ec47d8baed3eaf76ffca7826221e35

