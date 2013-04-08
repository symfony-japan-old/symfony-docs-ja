.. note::

    * 対象バージョン：2.2
    * 翻訳更新日：2013/04/09

フォームのレンダリングのカスタマイズ方法
========================================

Symfony は、フォームのレンダリングをカスタマイズする方法をいくつか用意しています。この記事では、テンプレートエンジンに Twig, PHP のどちらを使用しても、最小の努力で全てのフォームのパーツをカスタマイズする方法を学びます。

フォームレンダリングの基本
--------------------------

次のように ``form_row`` を使用することで、フォームフィールドのラベル、エラー、 HTML ウィジェットを簡単に表示することができるのを覚えてますでしょうか？

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['age']) ?>

また個々のフィールドを３つのパーツにしてそれぞれ表示することもできます。

.. configuration-block::

    .. code-block:: jinja

        <div>
            {{ form_label(form.age) }}
            {{ form_errors(form.age) }}
            {{ form_widget(form.age) }}
        </div>

    .. code-block:: php

        <div>
            <?php echo $view['form']->label($form['age']) ?>
            <?php echo $view['form']->errors($form['age']) ?>
            <?php echo $view['form']->widget($form['age']) ?>
        </div>

両方のケースで、 Symfony Standard Edition に付いてくるマークアップを使用して、フォームラベル、エラー、 HTML ウィジェットを表示します。例えば、上記の双方ともが以下のように表示されることになります。

.. code-block:: html

    <div>
        <label for="form_age">Age</label>
        <ul>
            <li>This field is required</li>
        </ul>
        <input type="number" id="form_age" name="form[age]" />
    </div>

素早くプロトタイプして、フォームをテストするには、全てのフォームを一行で表示することができます。

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form) ?>

このレシピの残りでは、フォームのマークアップの全てのパーツを、いろんな異なるレベルでどうやって変更するかについて説明します。一般的なフォームレンダリングに関しての詳細は、 :ref:`form-rendering-templates` を参照してください。

.. _cookbook-form-customization-form-themes:

フォームテーマとは何か？
------------------------

Symfony はフォームのフラグメントを使用します。フォームのフラグメントとは、フィールドのラベルやエラーや ``input`` 入力テキストフィールド、 ``select`` 選択リストタグなどのフォームの各部を構成するパーツ１つのみの小さなテンプレートです。

フラグメントは Twig では、ブロックとして、 PHP ではテンプレートファイルとして定義されます。

*theme* はフォームを表示する際に使用するフラグメントのセットに他なりません。つまり、フォームの表示の一部をカスタマイズするには、適切なフォームフラグメントのカスタマイズを含んだ *theme* をインポートすることになります。

Symfony は、デフォルトのテーマ (Twig の際は `form_div_layout.html.twig`_ 、 PHP の際は ``FrameworkBundle:Form`` ) が付いてきます。このデフォルトのテーマは、フォームの全てのパーツとして表示されるべき全てのフラグメントを定義します。

次のセクションでは、このフラグメントの一部、もしくは全部をどうやってオーバーライドしてテーマをカスタマイズするかを学びます。

例えば、 ``integer`` タイプフィールドのウィジェットが表示されると、 ``input`` ``number`` フィールドが生成されます。

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['age']) ?>

は次のように表示されます。

.. code-block:: html

    <input type="number" id="form_age" name="form[age]" required="required" value="33" />

内部的に、 Symfony はフィールドを表示するために ``integer_widget`` フラグメントを使用します。それは、フィールドタイプが ``integer`` で、 ``label`` や ``errors`` ではなく、この ``widget`` を表示しているからです。

Twig では、 `form_div_layout.html.twig`_ テンプレートの ``integer_widget`` ブロックをデフォルトとして使用します。

PHP では、 ``FrameworkBundle/Resources/views/Form`` フォルダの ``integer_widget.html.php`` ファイルを使用します。

``integer_widget`` フラグメントのデフォルトの実装は以下のようになっています。

.. configuration-block::

    .. code-block:: jinja

        {% block integer_widget %}
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        {% endblock integer_widget %}

    .. code-block:: html+php

        <!-- integer_widget.html.php -->

        <?php echo $view['form']->block($form, 'form_widget_simple', array('type' => isset($type) ? $type : "number")) ?>

上記を見ればわかるように、このフラグメント自体は、他のフラグメント ``form_widget_simple`` を表示しています。

.. configuration-block::

    .. code-block:: html+jinja

        {# form_div_layout.html.twig #}
        {% block form_widget_simple %}
            {% set type = type|default('text') %}
            <input type="{{ type }}" {{ block('widget_attributes') }} {% if value is not empty %}value="{{ value }}" {% endif %}/>
        {% endblock form_widget_simple %}

    .. code-block:: html+php

        <!-- FrameworkBundle/Resources/views/Form/form_widget_simple.html.php -->
        <input
            type="<?php echo isset($type) ? $view->escape($type) : 'text' ?>"
            <?php if (!empty($value)): ?>value="<?php echo $view->escape($value) ?>"<?php endif ?>
            <?php echo $view['form']->block($form, 'widget_attributes') ?>
        />

ポイントは、フラグメントがフォームのそれぞれの部分の HTML 出力を担っていることです。フォームの出力をカスタマイズするには、正しいフラグメントを確認して、オーバーライドするだけです。フォームフラグメントのセットのカスタマイズは、フォーム "theme" となります。フォームを表示する際に、適用したいテーマを選択することができます。

Twig では、テーマは、１つのテンプレートファイルになり、フラグメントは、そのファイルで定義されたブロックになります。

PHP では、テーマは、１つのフォルダになり、フラグメントは、そのフォルダ内の個々のテンプレートファイルになります。

.. _cookbook-form-customization-sidebar:

.. sidebar:: どのブロックをカスタマイズするか知る

    この例では、カスタマイズされたフラグメントの名前は、全ての ``integer`` フィールドタイプの HTML ``widget`` をオーバーライドすることになったので ``integer_widget`` になります。もしテキストエリアフィールドをカスタマイズすることになれば、 ``textarea_widget`` をカスタマイズすることになります。

    このようにフラグメントの名前は、フィールドタイプと ``widget``, ``label``, ``errors``, ``rows`` のように表示するフィールドのパーツを結合したものです。そのため、入力 ``text`` フィールドのエラーの表示をカスタマイズするには、 ``text_errors`` フラグメントをカスタマイズする必要があります。

    しかし、より一般的には、 *全て* のフィールドに渡ったエラーの表示方法をカスタマイズするときもあります。その際には、 ``form_errors`` フラグメントをカスタマイズしてください。これでフィールドタイプの継承ができます。 ``text`` タイプは ``field`` タイプから継承していますので、フォームコンポーネントは、 ``form_errors`` のような親フラグメントの名前を探す前に、 ``text_errors`` のような特定のタイプのフラグメントを探します。

    このトピックに関する詳細は、 :ref:`form-template-blocks` を参照してください。

.. _cookbook-form-theming-methods:

フォームをテーマ化する
----------------------

フォームのテーマ化のパワーを見るために、全ての入力 ``number`` フィールドを ``div`` タグでラップする例を見てみましょう。このためのポイントは、 ``integer_widget`` フラグメントのカスタマイズです。

Twig でフォームをテーマ化する
-----------------------------

Twig でフォームフィールドのブロックをカスタマイズする際に、カスタマイズしたフォームブロックを置く *場所* に関して２つのオプションがあります。

+--------------------------------------+-----------------------------------+-------------------------------------------+
| 方法                                 | メリット                          | デメリット                                |
+======================================+===================================+===========================================+
| フォームと同じテンプレートの中       | 速く簡単に可能                    | 他のテンプレートで再利用できない          |
+--------------------------------------+-----------------------------------+-------------------------------------------+
| 別のテンプレートの中                 | 多くのテンプレートで再利用可能    | 専用のテンプレートを作成しなければならない|
+--------------------------------------+-----------------------------------+-------------------------------------------+

両方の方法で、同じことが可能ですが、シチュエーションによってどちらが適切か異なります。

.. _cookbook-form-twig-theming-self:

方法 1: フォームと同じテンプレートの中
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``integer_widget`` ブロックをカスタマイズする最も簡単な方法は、実際にフォームを表示するテンプレートを直接カスタマイズすることです。

.. code-block:: html+jinja

    {% extends '::base.html.twig' %}

    {% form_theme form _self %}

    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        </div>
    {% endblock %}

    {% block content %}
        {# render the form #}

        {{ form_row(form.age) }}
    {% endblock %}

特別なタグの ``{% form_theme form _self %}`` を使えば、 Twig は同テンプレート中のオーバライドされたフォームブロックを探します。 ``form.age`` フィールドは ``integer`` タイプフィールドであると仮定すると、ウィジェットが表示される際に ``integer_widget`` ブロックが使用されます。

この方法のディスアドバンテージは、カスタマイズされたフォームブロックを他のテンプレートから再利用できないことです。つまり、この方法はアプリケーションで特別で単一なフォームのカスタマイズに便利になるのです。アプリケーションの他のフォームを横断してフォームのカスタマイズを再利用したい際には、次のセクションを読んでください。

.. _cookbook-form-twig-separate-template:

方法 2: 別のテンプレートの中
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

全く別のテンプレートの中にカスタマイズした ``integer_widget`` フォームブロックを入れることを選択することもできます。コードと最終的な結果は同じになりますが、これで多くのテンプレートを横断してフォームのカスタマイズが再利用できるようになります。

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}

    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        </div>
    {% endblock %}

カスタマイズしたフォームブロックを作成したので、 Symfony からそれを呼ぶようにしなければなりません。実際にフォームを表示するテンプレートの中で、 ``form_theme`` タグを通してこのテンプレートを呼び出します。

.. _cookbook-form-twig-theme-import-template:

.. code-block:: html+jinja

    {% form_theme form 'AcmeDemoBundle:Form:fields.html.twig' %}

    {{ form_widget(form.age) }}

``form.age`` ウィジェットが表示されるときに、 Symfony は、新しいテンプレートで ``integer_widget`` ブロックを使用します。そして、 ``input`` タグは、カスタマイズしたブロックで指定した ``div`` 要素で囲まれます。

.. _cookbook-form-php-theming:

PHP でフォームをテーマ化する
----------------------------

テンプレートエンジンとして、 PHP を使用する際に、フラグメントをカスタマイズする唯一の方法は、 Twig の２つ目の方法と同じように、新しくテンプレートファイルを作成することです。

テンプレートファイルは、フラグメントにちなんで名付ける必要があります。例えば、 ``integer_widget`` フラグメントをカスタマイズするには、 ``inter_widget.html.php`` を作成しなければなりません。

.. code-block:: html+php

    <!-- src/Acme/DemoBundle/Resources/views/Form/integer_widget.html.php -->

    <div class="integer_widget">
        <?php echo $view['form']->block('field_widget', array('type' => isset($type) ? $type : "number")) ?>
    </div>

これでカスタマイズされたフォームテンプレートを作成できましたので、 Symfony から使ってみましょう。実際にフォームを表示するテンプレートの中で、 ``setTheme`` ヘルパーメソッドを通してテーマを使用するようにします。

.. _cookbook-form-php-theme-import-template:

.. code-block:: php

    <?php $view['form']->setTheme($form, array('AcmeDemoBundle:Form')) ;?>

    <?php $view['form']->widget($form['age']) ?>

``form.age`` ウィジェットが表示されるときに、 Symfony はカスタマイズされた ``integer_widget.html.php`` テンプレートを使用し、 ``input`` タグは ``div`` 要素でラップされます。

.. _cookbook-form-twig-import-base-blocks:

ベースフォームブロックの参照(Twig のみ)
---------------------------------------

これまで、特定のフォームブロックをオーバーライドするのにベストな方法は、 `form_dev_layout.html.twig`_ のデフォルトブロックをコピーして、カスタマイズして異なるテンプレートにペーストすることでした。多くのケースでは、カスタマイズするときにベースブロックを参照してこれを避けることができます。

これは簡単にすることができますが、フォームブロックのカスタマイズがフォームと同じテンプレートにあるか、または別のテンプレートにあるかによって多少異なります。

フォームと同じテンプレートの中からブロックを参照する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

フォームを表示しているテンプレートの中で ``use`` タグを追加してブロックをインポートします。

.. code-block:: jinja

    {% use 'form_div_layout.html.twig' with integer_widget as base_integer_widget %}

これで `form_div_layout.html.twig`_ のブロックがインポートされたら、 ``integer_widget`` ブロックを ``base_integer_widget`` として呼びます。これは、 ``integer_widget`` ブロックを再定義することになり、 ``base_integer_widget`` を通してデフォルトのマークアップを参照できます。

.. code-block:: html+jinja

    {% block integer_widget %}
        <div class="integer_widget">
            {{ block('base_integer_widget') }}
        </div>
    {% endblock %}

外部のテンプレートからベースブロックを参照する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

外部テンプレートにカスタマイズしたフォームを作成していれば、Twig の  ``parent()`` 関数を使用してベースブロックを参照することができます。

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}

    {% extends 'form_div_layout.html.twig' %}

    {% block integer_widget %}
        <div class="integer_widget">
            {{ parent() }}
        </div>
    {% endblock %}

.. note::

    テンプレートエンジンとして PHP を使用している際には、ベースブロックを参照することはできません。その際には、ベースブロックから手動でコピーして、新しいテンプレートファイルにペーストする必要があります。

.. _cookbook-form-global-theming:

アプリケーション全体のカスタマイズ
----------------------------------

アプリケーション全体でグローバルにフォームをカスタマイズしたいときは、外部テンプレートとしてフォームカスタマイズを作成し、アプリケーションのコンフィギュレーション内でインポートすることによって、実現できます。

Twig
~~~~

次のコンフィギュレーションを使用すれば、 ``AcmeDemoBundle:Form:fields.html.twig`` テンプレート内の全てのカスタマイズされたフォームブロックを、フォームが表示されるときにグローバルに使用することができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeDemoBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>AcmeDemoBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'AcmeDemoBundle:Form:fields.html.twig',
             ))
            // ...
        ));

デフォルトでは、 Twig はフォーム表示に *div* レイアウトを使用します。しかし、人によっては、 *table* レイアウトでのフォーム表示を好むかもしれません。そのときは、レイアウトに ``form_table_layout.html.twig`` リソースを使用してください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources: ['form_table_layout.html.twig']
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>form_table_layout.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'form_table_layout.html.twig',
             ))
            // ...
        ));

テンプレートを１つだけ変更したい際には、リソースとしてテンプレートを追加するのではなく、次の行をテンプレートに追加してください。

.. code-block:: html+jinja

	{% form_theme form 'form_table_layout.html.twig' %}

上記のコードの ``form`` 変数は、テンプレートに渡すフォームビューの変数であること覚えておいてください。

PHP
~~~

次のコンフィギュレーションを使用すれば、フォームが表示されるときに ``src/Acme/DemoBundle/Resources/views/Form`` フォルダの内部のカスタマイズされたフォームフラグメントがグローバルに使用されます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'AcmeDemoBundle:Form'
            # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeDemoBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>


    .. code-block:: php

        // app/config/config.php

        // PHP
        $container->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeDemoBundle:Form',
             )))
            // ...
        ));

デフォルトでは、 PHP エンジンは、フォーム表示に *div* レイアウトを使用します。しかし、人によっては、 *table* レイアウトでのフォーム表示を好むかもしれません。そのときは、レイアウトに ``FrameworkBundle:FormTable`` リソースを使用してください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'FrameworkBundle:FormTable'

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>FrameworkBundle:FormTable</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'FrameworkBundle:FormTable',
             )))
            // ...
        ));

テンプレートを１つだけ変更したい際には、リソースとしてテンプレートを追加するのではなく、次の行をテンプレートに追加してください。

.. code-block:: html+php

	<?php $view['form']->setTheme($form, array('FrameworkBundle:FormTable')); ?>

上記のコードの ``$form`` 変数は、テンプレートに渡すフォームビューの変数であること覚えておいてください。

個々のフィールドのカスタマイズ
------------------------------

これまで、全てのテキストフィールドタイプのウィジェットの出力の異なるカスタマイズ方法を見ていました。個々のフィールドもカスタマイズすることができます。例えば、 ``first_name`` と ``last_name`` のように ``text`` フィールドが２つあるが、どちらかしかカスタマイズをしたくないときを想定します。これは、フィールドの id 属性とカスタマイズされるフィールドの部分を結合した名前のフラグメントをカスタマイズすることによってできます。

.. configuration-block::

    .. code-block:: html+jinja

        {% form_theme form _self %}

        {% block _product_name_widget %}
            <div class="text_widget">
                {{ block('form_widget_simple') }}
            </div>
        {% endblock %}

        {{ form_widget(form.name) }}

    .. code-block:: html+php

        <!-- Main template -->

        <?php echo $view['form']->setTheme($form, array('AcmeDemoBundle:Form')); ?>

        <?php echo $view['form']->widget($form['name']); ?>

        <!-- src/Acme/DemoBundle/Resources/views/Form/_product_name_widget.html.php -->

        <div class="text_widget">
              echo $view['form']->renderBlock('form_widget_simple') ?>
        </div>

ここで、 ``_product_name_widget`` フラグメントが *id* が ``product_name`` であるテンプレートを定義します(そして、 name 属性は ``product[name]`` になります)。

.. tip::

   フィールドの ``product`` の部分は、フォームの名前になります。これは、フォームタイプ名に基づいて、手動で設定、もしくは自動生成によって付けられます( ``ProductType`` は ``product`` になるように)。フォームの名前がわからなければ、生成されたフォームのソースを参照してください。

同じメソッドを使用してフィールド列全体のマークアップをオーバーライドすることもできます。

.. configuration-block::

    .. code-block:: html+jinja

        {% form_theme form _self %}

        {% block _product_name_row %}
            <div class="name_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endblock %}

    .. code-block:: html+php

        <!-- _product_name_row.html.php -->

        <div class="name_row">
            <?php echo $view['form']->label($form) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form) ?>
        </div>

他の一般的なカスタマイズに関して
--------------------------------

これまでのレシピで、フォームを表示する一部をカスタマイズするいくつかの異なる方法を見てきました。ポイントは、制御したいフォームの部分に対応するフラグメントをカスタマイズすることでした(:ref:`naming form blocks<cookbook-form-customization-sidebar>` を参照してください)。


次のセクションでは、いくつかの共通のフォームのカスタマイズについて見ていきます。これらのカスタマイズを適用するには、 :ref:`cookbook-form-theming-methods` セクションに記述されたメソッドを使用してください。

エラー出力をカスタマイズする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
   フォームのコンポーネントは、 *どうやって* バリデーションエラーを表示するかのみを扱い、実際のバリデーションエラーメッセージに関しては決定権はありません。エラーメッセージは、オブジェクトに適用したバリデーション制約によって、決められます。詳細は、 :doc:`validation</book/validation>` を参照してください。

フォームがエラーを検知した際に、多くの異なる方法でエラーの表示をカスタマイズできます。フィールドのエラーメッセージは、 ``form_errors`` ヘルパーを使用することで、表示されます。

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->errors($form['age']); ?>

デフォルトでは、エラーは、順序の関係の無いリストで表示されます。

.. code-block:: html

    <ul>
        <li>必須項目です。</li>
    </ul>

*全て* のフィールドでエラーの表示をオーバーライドするには、単に ``form_errors`` フラグメントをコピーアンドペーストして、カスタマイズします。

.. configuration-block::

    .. code-block:: html+jinja

        {# form_errors.html.twig #}
        {% block form_errors %}
            {% spaceless %}
                {% if errors|length > 0 %}
                <ul>
                    {% for error in errors %}
                        <li>{{ error.message }}</li>
                    {% endfor %}
                </ul>
                {% endif %}
            {% endspaceless %}
        {% endblock form_errors %}

    .. code-block:: html+php

        <!-- form_errors.html.php -->
        <?php if ($errors): ?>
            <ul>
                <?php foreach ($errors as $error): ?>
                    <li><?php echo $error->getMessage() ?></li>
                <?php endforeach; ?>
            </ul>
        <?php endif ?>

.. tip::
    このカスタマイズの適用方法の詳細は、 :ref:`cookbook-form-theming-methods` を参照してください。

特定のフィールドタイプのみのエラー出力をカスタマイズすることもできます。例えば、フォームのよりグローバルな特定のエラーは、デフォルトでは、フォームの一番上に表示されますが、別々に表示させることができます。

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form) }}

    .. code-block:: php

        <?php echo $view['form']->render($form); ?>

これらのエラーのマークアップ *のみ* をカスタマイズするには、上記のように同じ方法に従ってください。しかし、 Twig の際は ``form_errors`` ブロックを呼んで、 PHP の際は ``form_errors.html.php`` ファイルを呼ぶことになります。これで、 ``form`` タイプのエラーが表示されれば、カスタマイズされたフラグメントがデフォルトの ``form_errors`` の代わりに使用されます。

"Form Row" をカスタマイズする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

可能であれば、フォームフィールドの表示の最も簡単な方法は、 ``form_row`` 関数を使用することです。 ``form_row`` 関数は、フィールドのラベル、エラー、 HTML ウィジェットを表示します。 *全て* のフォームフィールドの並びの表示のマークアップをカスタマイズするために、 ``form_row`` フラグメントをオーバーライドします。例えばそれぞれの並びを ``div`` 要素で囲みたいとします。

.. configuration-block::

    .. code-block:: html+jinja

        {# form_row.html.twig #}
        {% block form_row %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endblock form_row %}

    .. code-block:: html+php

        <!-- form_row.html.php -->

        <div class="form_row">
            <?php echo $view['form']->label($form) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form) ?>
        </div>

.. tip::
    このカスタマイズの適用方法の詳細は、 :ref:`cookbook-form-theming-methods` を参照してください。

"Required" のアスタリスクをフィールドラベルに追加する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

全ての入力必須なフィールドにアスタリスク(``*``)の印を付けるには、 ``form_label`` フラグメントをカスタマイズします。

Twig を使用した際に、フォームと同じテンプレート内でフォームのカスタマイズをするには、 ``use`` タグを変更して、次のように加えてください。

.. code-block:: html+jinja

    {% use 'form_div_layout.html.twig' with form_label as base_form_label %}

    {% block field_label %}
        {{ block('base_form_label') }}

        {% if required %}
            <span class="required" title="必須項目です">*</span>
        {% endif %}
    {% endblock %}

Twig を使用した際に、別のテンプレート内でフォームのカスタマイズをする際には、次のようにしてください。

.. code-block:: html+jinja

    {% extends 'form_div_layout.html.twig' %}

    {% block form_label %}
        {{ parent() }}

        {% if required %}
            <span class="required" title="必須項目です">*</span>
        {% endif %}
    {% endblock %}

テンプレートエンジンに PHP を使用している際は、オリジナルのテンプレートから内容をコピーしてこなければなりません。

.. code-block:: html+php

    <!-- form_label.html.php -->

    <!-- original content -->
    <?php if ($required) { $label_attr['class'] = trim((isset($label_attr['class']) ? $label_attr['class'] : '').' required'); } ?>
    <?php if (!$compound) { $label_attr['for'] = $id; } ?>
    <?php if (!$label) { $label = $view['form']->humanize($name); } ?>
    <label <?php foreach ($label_attr as $k => $v) { printf('%s="%s" ', $view->escape($k), $view->escape($v)); } ?>><?php echo $view->escape($view['translator']->trans($label, array(), $translation_domain)) ?></label>

    <!-- customization -->
    <?php if ($required) : ?>
        <span class="required" title="必須項目です">*</span>
    <?php endif ?>

.. tip::
    このカスタマイズの適用方法の詳細は、 :ref:`cookbook-form-theming-methods` を参照してください。

"help" メッセージを追加する
~~~~~~~~~~~~~~~~~~~~~~~~~~~

フォームウィジェットのオプションの "help" メッセージもカスタマイズすることができます。

Twig を使用した際に、フォームと同じテンプレート内でフォームのカスタマイズをするには、 ``use`` タグを変更して、次のように加えてください。

.. code-block:: html+jinja

    {% use 'form_div_layout.html.twig' with form_widget_simple as base_form_widget_simple %}

    {% block form_widget_simple %}
        {{ block('base_form_widget_simple') }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

Twig を使用した際に、別のテンプレート内でフォームのカスタマイズをする際には、次のようにしてください。

.. code-block:: html+jinja

    {% extends 'form_div_layout.html.twig' %}

    {% block form_widget_simple %}
        {{ parent() }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

テンプレートエンジンに PHP を使用した際は、オリジナルのテンプレートから内容をコピーしなければなりません。

.. code-block:: html+php

    <!-- form_widget_simple.html.php -->

    <!-- Original content -->
    <input
        type="<?php echo isset($type) ? $view->escape($type) : 'text' ?>"
        <?php if (!empty($value)): ?>value="<?php echo $view->escape($value) ?>"<?php endif ?>
        <?php echo $view['form']->block($form, 'widget_attributes') ?>
    />

    <!-- Customization -->
    <?php if (isset($help)) : ?>
        <span class="help"><?php echo $view->escape($help) ?></span>
    <?php endif ?>

フィールドの下にヘルプメッセージを表示するには、 ``help`` 変数を渡してください。

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form.title, { 'help': 'foobar' }) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['title'], array('help' => 'foobar')) ?>

.. tip::
    
    このカスタマイズの適用方法の詳細は、 :ref:`cookbook-form-theming-methods` を参照してください。

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig

.. 2012/01/10 ganchiku 78fbe0505f42b091eca4dd42b780291e3eed950d

