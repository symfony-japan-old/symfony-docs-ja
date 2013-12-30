preferred_choices
~~~~~~~~~~~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

このオプションが指定されていた場合、すべての選択肢の一部がセレクトメニューの最上部に移動します。以下は "Baz" オプションが最上部に移動し、それと残りの選択肢の間にセパレーターがつきます。::

    $builder->add('foo_choices', 'choice', array(
        'choices' => array('foo' => 'Foo', 'bar' => 'Bar', 'baz' => 'Baz'),
        'preferred_choices' => array('baz'),
    ));

ここで注意すべきは好みの選択肢は ``select`` 要素で表示されたときにのみ意味を持つことです（すなわち ``expanded`` は ``false`` ）。この好みの選択肢と通常の選択肢は点線（すなわち ``-------------------`` ）によって明示的に分離されます。これは表示する際に編集することが出来ます。:

.. configuration-block::

    .. code-block:: jinja
    
        {{ form_widget(form.foo_choices, { 'separator': '=====' }) }}

    .. code-block:: php
    
        <?php echo $view['form']->widget($form['foo_choices'], array('separator' => '=====')) ?>

.. 2013/12/28 yositani2002 906e345d04b170d3367cfc8e523239137867db0d
